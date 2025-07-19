#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "usart.h"
#include "st7735.h"
#include "stfonts.h"
#include "stimage.h"


static void board_lowlevel_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

int main(void)
{
    board_lowlevel_init();

    led_init();
    usart_init();
    st7735_init();

    st7735_fill_screen(ST7735_GREEN);

    uint32_t i = 0;
    char str[32];
    while (1)
    {
        sprintf(str, "%04u", i++);
        i = i % 10000; // 0 ~ 9999
        st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_RED, ST7735_GREEN);
        st7735_write_fonts(0, 20, &font_ni_hao_shi_jie_16x16, 0, 4, ST7735_RED, ST7735_GREEN);
        delay(1000);
    }
}
