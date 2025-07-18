#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include <string.h>
#include "main.h"
#include "led.h"
#include "usart.h"

static uint8_t data_buffer[16];
static uint16_t data_len;
static bool usart_tx_busy = false;

static void usart_received(uint8_t data)
{
	if (data_len < 15)
	{
		data_buffer[data_len] = data;
		data_len++;
	}
}

static void on_usart_send_finish(void)
{
	usart_tx_busy = false;
}

static void wait_usart_send_finish(void)
{
	while (usart_tx_busy == true)
		;
}

#define write_string(_str)           \
	do                                 \
	{                                  \
		usart_tx_busy = true;            \
		usart_write_string(_str, false); \
		wait_usart_send_finish();        \
	} while (0);

#define write_data(_data, _len)           \
	do                                      \
	{                                       \
		usart_tx_busy = true;                 \
		usart_write_data(_data, _len, false); \
		wait_usart_send_finish();             \
	} while (0);

int main(void)
{

	board_init();
	led_init();
	usart_init();
	usart_send_finish_register(on_usart_send_finish);
	usart_receive_register(usart_received);

	while (true)
	{

		if (data_len > 0)
		{
			write_string("Received: ");
			write_data(data_buffer, data_len);
			write_string("\r\n");
			data_len = 0;
		}
	}
}
