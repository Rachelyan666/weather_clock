#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "rtc.h"
#include "esp_at.h"
#include "st7735.h"
#include "stfonts.h"
#include "weather.h"


static const char *wifi_ssid = "YXC";
static const char *wifi_password = "yxc19951024";
static const char *weather_uri = "https://api.seniverse.com/v3/weather/now.json?key=SEJICVxMVvVm92Yp3&location=hefei&language=en&unit=c";


static void board_lowlevel_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
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

int main(void)
{
    board_lowlevel_init();

    led_init();
    rtc_init();

    st7735_init();
    st7735_fill_screen(ST7735_BLACK);

    delay_ms(2000);

    if (!esp_at_init())
    {
        while (1);
    }
    if (!esp_at_wifi_init())
    {
        while (1);
    }
    if (!esp_at_wifi_connect(wifi_ssid, wifi_password))
    {
        while (1);
    }

    bool weather_ok = false;
    bool sntp_ok = false;
    uint32_t t = 0;
    char str[64];
    while (1)
    {
        t++;
        delay_ms(1000);

        rtc_date_t date;
        rtc_get_date(&date);
        snprintf(str, sizeof(str), "%04d-%02d-%02d", date.year, date.month, date.day);
        st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
        snprintf(str, sizeof(str), "%02d:%02d:%02d", date.hour, date.minute, date.second);
        st7735_write_string(0, 16, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);


        if (!weather_ok || t % 60 == 0)
        {
            const char *rsp;
            weather_ok = esp_at_http_get(weather_uri, &rsp, NULL, 10000);
            weather_t weather;
            weather_parse(rsp, &weather);

            snprintf(str, sizeof(str), "%s", "hefei");
            st7735_write_string(0, 64, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
            snprintf(str, sizeof(str), "%s", weather.weather);
            st7735_write_string(0, 80, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
            snprintf(str, sizeof(str), "%s", weather.temperature);
            st7735_write_string(0, 96, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
        }

        if (!sntp_ok || t % 3600 == 0)
        {
            uint32_t ts;
            sntp_ok = esp_at_sntp_init();
            delay_ms(1000);
            esp_at_time_get(&ts);
            rtc_set_timestamp(ts + 8 * 60 * 60);
        }
    }
}
