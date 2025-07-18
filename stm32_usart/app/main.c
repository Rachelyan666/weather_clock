#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "usart.h"


int main(void){

	board_init();
	led_init();
	usart_init();
	
	while(true){
		usart_write_string("hello world\r\n");
		led_on();
		delay(200);
		led_off();
		delay(200);
	}
}
