#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "key_exti.h"


static void on_key_pressed(void){
	led_toggle();
}

int main(void){

	board_init();
	led_init();
	key_exti_init();
	key_exti_register(on_key_pressed);
	
	while(true){
		;
	}
}
