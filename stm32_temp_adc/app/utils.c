#include <stdint.h>
#include "stm32f10x.h"

void delay_ms(uint32_t x)
{
	for (uint32_t i = 0; i < x; i++)
	{
		for (uint32_t j = 0; j < 7200; j++)
		{
			// 72000 is per ms
		}
	}
}

void board_init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // OPEN THE CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // OPEN THE CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // OPEN THE CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	// enable usart clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}
