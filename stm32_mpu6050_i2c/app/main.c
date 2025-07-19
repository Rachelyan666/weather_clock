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


int main(void)
{
    board_init();

    led_init();
    usart_init();
    st7735_init();

    st7735_fill_screen(ST7735_BLACK);

    uint32_t i = 0;
    char str[32];
    while (1)
    {
        sprintf(str, "%04u", i++);
        i = i % 10000; // 0 ~ 9999
        st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
        //st7735_write_fonts(0, 20, &font_ni_hao_shi_jie_16x16, 0, 4, ST7735_GREEN, ST7735_BLACK);
        st7735_write_fonts(0, 20, &font_hello_world_8x16, 0, 11, ST7735_GREEN, ST7735_BLACK);
        st7735_draw_image(0, 40, image_tv_128x72.width, image_tv_128x72.height, (uint8_t *)image_tv_128x72.data);
        delay(1000);
    }
}
