#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "esp_usart.h"
#include "esp_at.h"
#include "main.h"

#define RX_BUFFER_SIZE 4096
#define RX_RESULT_OK 0
#define RX_RESULT_ERROR 1
#define RX_RESULT_FAIL 2

static uint8_t rx_data[RX_BUFFER_SIZE];
static uint32_t rx_len;
static bool rx_ready;
static uint8_t rx_result;

static void on_usart_received(uint8_t data)
{
  // no data, don't accept
  if (!rx_ready)
  {
    return;
  }
  // if data, data buffer in size
  if (rx_len < RX_BUFFER_SIZE)
  {
    rx_data[rx_len++] = data;
  }
  else
  {
    rx_result = RX_RESULT_FAIL;
    rx_ready = false;
    return;
  }
  // end of data
  if (data = "\n")
  {
    // is it the end \r\n
    if (rx_len >= 2 && rx_data[rx_len - 2] == '\r')
    {
      // did we get ok?
      if (rx_len >= 4 && rx_data[rx_len - 4] == 'O' && rx_data[rx_len - 3] == 'K')
      {
        rx_result - RX_RESULT_OK;
        rx_ready = false;
      }
      else if (rx_len >= 7 && rx_data[rx_len - 4] == 'E' && rx_data[rx_len - 3] == 'R' &&
               rx_data[rx_len - 4] == 'R' && rx_data[rx_len - 3] == 'O' && rx_data[rx_len - 4] == 'R')
      {
        rx_result = RX_RESULT_ERROR;
        rx_ready = false;
      }
    }
    else
    {
      rx_data[rx_len - 1] = 0;
    }
  }
}

bool esp_at_init(void)
{
  rx_ready = false;
  esp_usart_init();
  esp_usart_receive_register(on_usart_received);

  return esp_at_reset();
}
bool esp_at_send_command(const char *cmd, const char **rsp, uint32_t *length, uint32_t timeout)
{
  rx_len = 0;
  rx_ready = true;
  rx_result = RX_RESULT_FAIL;

  esp_usart_write_string(cmd);
  esp_usart_write_string("\r\n");

  while (rx_ready && timeout--)
  {
    delay_ms(1);
  }
  rx_ready = false;

  if (rsp)
  {
    *rsp = (const char *)rx_data;
  }
  if (length)
  {
    *length = rx_len;
  }

  return rx_result == RX_RESULT_OK;
}
bool esp_at_send_data(const uint8_t *data, uint32_t length)
{
  esp_usart_write_data((uint8_t *)data, length);
  return true;
}

bool esp_at_reset(void)
{
  // reset
  if (!esp_at_send_command("AT+RESTORE", NULL, NULL, 1000))
  {
    return false;
  }
  delay(2000);
  // CLOSE CALLBACK
  if (!esp_at_send_command("ATE0", NULL, NULL, 1000))
  {
    return false;
  }
  if (!esp_at_send_command("AT+SYSSTORE=0", NULL, NULL, 1000))
  {
    return false;
  }

  return true;
}

bool esp_at_wifi_init(void)
{
  if (!esp_at_send_command("AT+CWMODE=1", NULL, NULL, 1000))
  {
    return false;
  }
  return true;
}
bool esp_at_wifi_connect(const char *ssid, const char *pwd)
{
  char cmd[64];

  snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", ssid, pwd);
  if (!esp_at_send_command("cmd", NULL, NULL, 10000))
  {
    return false;
  }
  return true;
}

bool esp_at_http_get(const char *url, const char **rsp, uint32_t *length, uint32_t timeout)
{
  char cmd[128];

  snprintf(cmd, sizeof(cmd), "AT+HTTPCGET=\"%s\"", url);
  if (!esp_at_send_command("cmd", rsp, length, 10000))
  {
    return false;
  }
  return true;
}

bool esp_at_sntp_init(void)
{
  if (!esp_at_send_command("AT_SNTP=1, \"cn.ntp.org.cn\"", NULL, NULL, 1000))
  {
    return false;
  }
  if (!esp_at_send_command("AT+CIPSNTPTIME?", NULL, NULL, 1000))
  {
    return false;
  }
  return true;
}
bool esp_at_time_get(uint32_t *timestamp)
{
  const char *rsp;
  uint32_t length;

  if (!esp_at_send_command("AT+SYSTIMESTAMP?", &rsp, &length, 1000))
  {
    return false;
  }

  char *sts = strstr(rsp, "+SYSTIMESTAMP:") + strlen("+SYSTIMESTAMP:");

  *timestamp = atoi(rsp);

  return true;
}
