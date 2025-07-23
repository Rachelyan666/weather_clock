#ifndef __ESP_AT_H__
#define __ESP_AT_H__


#include <stdbool.h>
#include <stdint.h>


bool esp_at_init(void);
bool esp_at_send_command(const char *cmd, const char **rsp, uint32_t *length, uint32_t timeout);
bool esp_at_send_data(const uint8_t *data, uint32_t length);

bool esp_at_reset(void);

bool esp_at_wifi_init(void);
bool esp_at_wifi_connect(const char *ssid, const char *pwd);

bool esp_at_http_get(const char *url, const char **rsp, uint32_t *length, uint32_t timeout);

bool esp_at_sntp_init(void);
bool esp_at_time_get(uint32_t *timestamp);


#endif /* __ESP_AT_H__ */
