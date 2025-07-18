#include <stdint.h>
#include "stm32f10x.h"

void delay(uint32_t x){
	for (uint32_t i = 0; i < x; i++){
		for (uint32_t j = 0; j < 7200; j++){
			//72000 is per ms
		}
	}
}

void board_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //OPEN GPIOc CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //OPEN GPIOa CLOCK
	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}
