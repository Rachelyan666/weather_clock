#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"

static void delay(uint32_t x){
	for (uint32_t i = 0; i < x; i++){
		for (uint32_t j = 0; j < 72000; j++){
			//72000 is per ms
		}
	}
}

int main(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //OPEN THE CLOCK
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	
	while(true){
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(200);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(200);
	}
}
