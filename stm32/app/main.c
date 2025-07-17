#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"


int main(void){

	board_init();
	led_init();
	
	while(true){
		for (int i = 0; i < 1000; i++){
			for (int t = 0; t < 10; t++){
				for (int j = 0; j < i; j++);
				led_on();
				for (int j = 1000; j > i; j--);
				led_off();
			}
		}
		
		for (int i = 1000; i > 0; i--){
			for (int t = 0; t < 10; t++){
				for (int j = 0; j < i; j++);
				led_on();
				for (int j = 1000; j > i; j--);
				led_off();
			}
		}
	}
}
