#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "timer.h"
#include "pwm.h"
#include "rtc.h"
#include "st7735.h"
#include "stfonts.h"


static void board_lowlevel_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    BKP_DeInit();

    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
}

static void on_timer_elapsed(void)
{
    static uint32_t counter = 0;

    if (++counter == 500)
    {
        counter = 0;
        led_toggle();
    }
}

int main(void)
{
    board_lowlevel_init();

    led_init();

    st7735_init();
    st7735_fill_screen(ST7735_BLACK);

    timer_init(1000);
    timer_elapsed_register(on_timer_elapsed);
    timer_start();

    // pwm_init(1000);
    // pwm_set_duty(1, 100);
    // pwm_set_duty(2, 200);
    // pwm_set_duty(3, 300);
    // pwm_set_duty(4, 400);
    // pwm_start();

    rtc_init();
    rtc_date_t date = {2024, 8, 29, 22, 59, 24};
    rtc_set_date(&date);

    char str[64];
    while (1)
    {
        delay_ms(100);
        rtc_get_date(&date);

        sprintf(str, "Year: %u", date.year);
        st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
        sprintf(str, "Month: %u", date.month);
        st7735_write_string(0, 16, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
        sprintf(str, "Day: %u", date.day);
        st7735_write_string(0, 32, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
        sprintf(str, "Hour: %u", date.hour);
        st7735_write_string(0, 48, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
        sprintf(str, "Minute: %u", date.minute);
        st7735_write_string(0, 64, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
        sprintf(str, "Second: %u", date.second);
        st7735_write_string(0, 80, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
    }
}
