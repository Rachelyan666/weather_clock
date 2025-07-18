#include <stdbool.h>
#include "stm32f10x.h"
#include "key.h"


#define KEY_PORT GPIOB
#define KEY_PIN GPIO_Pin_10


void key_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //input up
	GPIO_InitStructure.GPIO_Pin = KEY_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

bool key_pressed(void){
	return GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == Bit_RESET;
}

void key_wait(void){
	while (key_pressed());
}
