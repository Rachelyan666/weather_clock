#include <stdint.h>
#include "stm32f10x.h"

void board_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //OPEN THE CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //OPEN THE CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //OPEN THE CLOCK

	//enable timer
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //OPEN THE CLOCK
}

void delay_us(uint32_t us){
	SysTick->LOAD = us * (SystemCoreClock / 1000000) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL = 0;
}

void delay_ms(uint32_t ms){
	while (ms --){
        delay_us(1000);
    }
}
