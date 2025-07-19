#include <stdint.h>
#include "stm32f10x.h"

void delay(uint32_t x){
	for (uint32_t i = 0; i < x; i++){
		for (uint32_t j = 0; j < 7200; j++){
			//72000 is per ms
		}
	}
}

void delay_us(uint32_t us){
	SysTick->LOAD = us * (SystemCoreClock / 1000000) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while ( (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL = 0;
}

void delay_ms(uint32_t ms){
	while (ms --){
        delay_us(1000);
    }
}


void board_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //OPEN THE CLOCK
}
