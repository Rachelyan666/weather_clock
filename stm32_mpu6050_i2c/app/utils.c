#include <stdint.h>
#include "stm32f10x.h"


void delay(uint32_t ticks)
{
    for (uint32_t t = 0; t < ticks; t++)
    {
        for (uint32_t n = 0; n < 7200; n++)
        {
            __NOP();
        }
    }
}

void board_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

