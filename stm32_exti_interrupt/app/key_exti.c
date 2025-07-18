#include <stdbool.h>
#include "stm32f10x.h"
#include "key_exti.h"
#include "main.h"


#define KEY_PORT GPIOB
#define KEY_PIN GPIO_Pin_10

#define KEY_EXTI_PORT_SRC GPIO_PortSourceGPIOB
#define KEY_EXTI_PIN_SRC GPIO_PinSource10
#define KEY_EXTI_LINE EXTI_Line10

static key_press_callback_t key_press_callback;


//private
static bool key_exti_pressed(void){
	return GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == Bit_RESET;
}

static void key_exti_wait(void){
	while (key_exti_pressed());
}


void key_exti_init(void){

	/* Configure PB.10 pin as input floating */
	GPIO_InitTypeDef   GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = KEY_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);

	/* Connect EXTI0 Line to PB.10 pin */
	GPIO_EXTILineConfig(KEY_EXTI_PORT_SRC, KEY_EXTI_PIN_SRC);

	/* Configure EXTI10 line */
	EXTI_InitTypeDef   EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = KEY_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI10 Interrupt to the lowest priority */
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void key_exti_register(key_press_callback_t callback){
	key_press_callback = callback;
}

void EXTI15_10_IRQHandler(void){
	if (key_exti_pressed()){
		delay(10);
		if (key_exti_pressed()){
			if (key_press_callback){
				key_press_callback();
			}
			key_exti_wait();
			delay(10);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line10); //CLEAR SO THAT WE CAN EXIT
}
