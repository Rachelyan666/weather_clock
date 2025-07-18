#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include <string.h>
#include "main.h"
#include "led.h"
#include "usart.h"


static uint8_t data_buffer[16];
static uint16_t data_len;


static void usart_received(uint8_t data){
	if (data_len < 15){
		data_buffer[data_len] = data;
		data_len++;
	}
}

int main(void){

	board_init();
	led_init();
	usart_init();
	usart_receive_register(usart_received);

	const char str[] = "hello world\n";
	while(true){

		usart_write_async((uint8_t *)str, strlen(str));
		delay(1000);

		// if (data_len > 0){
		// 	usart_write_string("Received: ");
		// 	usart_write_data(data_buffer, data_len);
		// 	data_len = 0;
		// 	usart_write_string("\r\n");
		// }
	}
}
