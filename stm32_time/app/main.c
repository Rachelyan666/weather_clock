#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "timer.h"

static void on_timer_elapsed(){
	static uint32_t counter = 0;
	
	if (counter == 500){
		counter = 0;
		led_toggle();
	}
	counter++;
}

int main(void){

	board_init();
	led_init();
	timer_init(1000);
	timer_elapsed_register(on_timer_elapsed);
	timer_start();
	
	while(true){
		// led_toggle();
		// delay_ms(500);
	}
}
