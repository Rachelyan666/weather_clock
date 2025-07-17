#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"


int main(void){

	board_init();
	led_init();
	
	bool led_state = false;
	while(true){
		led_state = !led_state;
		led_set(led_state);
		delay(200);
	}
}
