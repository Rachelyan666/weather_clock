#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "usart.h"
#include "ntc.h"

static uint8_t data_buffer[16];
static uint16_t data_len;

int main(void)
{

	board_init();

	usart_init();
	ntc_init();

	char sbuf[64];
	while (true)
	{
		delay(100);
		uint16_t ntc = ntc_read();
		//snprintf(sbuf, sizeof(sbuf), "NTC: 0x%04x\n", ntc);
		snprintf(sbuf, sizeof(sbuf), "NTC: %u\n", ntc);
		usart_write_string(sbuf);
	}
}
