#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"


int main(void){

	board_init();
	led_init();
	
	while(true){
		led_toggle();
		delay_ms(500);
	}
}
