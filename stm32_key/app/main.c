#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "key.h"


int main(void){

	board_init();
	led_init();
	key_init();
	
	while(true){
		if (key_pressed()){
			delay(10);
			if (key_pressed()){
				led_toggle();
				key_wait();
				delay(10); //bpth delay for debouncing
			}
		}
	}
}
