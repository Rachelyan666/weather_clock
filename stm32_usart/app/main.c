#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "usart.h"


static uint8_t data_buffer[16];
static uint16_t data_len;


static void usart_received(uint8_t data){
	if (data_len < 15){
		data_len++;
		data_buffer[data_len] = data;
	}
}

int main(void){

	board_init();
	led_init();
	usart_init();
	usart_receive_register(usart_received);
	
	while(true){
		if (data_len > 0){
			usart_write_string("Received: ");
			usart_write_data(data_buffer, data_len);
			data_len = 0;
			usart_write_string("\r\n");
		}
	}
}
