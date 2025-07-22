#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "rtc.h"
#include "st7735.h"
#include "stfonts.h"
#include "stimage.h"
#include "lcd_spi.h"
#include "esp_at.h"
#include "weather.h"

// pin connection
// esp32 gpio 6 -> stm32 A2
// esp32 gpio 7 -> stm32 A3
// gnd to gnd

static const char *wifi_ssid = "CTT-WIFI";
static const char *wifi_pswd = "U5GWBV6P";
static const char *weather_uri = "https://api.seniverse.com/v3/weather/now.json?key=SCKLd5GJ3KBdUvIAh&location=beijing&language=en&unit=c";

int main(void)
{

	board_init();
	led_init();
	rtc_init();

	st7735_init();
	st7735_fill_screen(ST7735_BLACK);

	delay_ms(5000);

	if (!esp_at_init())
	{
		while (1)
			;
	}
	if (!esp_at_wifi_init())
	{
		while (1)
			;
	}
	if (!esp_at_wifi_connect(wifi_ssid, wifi_pswd))
	{
		while (1)
			;
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
		snprintf(str, sizeof(str), "%04d/%02d/%2d", date.year, date.month, date.day);
		st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
		snprintf(str, sizeof(str), "%02d:%02d:%02d", date.hour, date.minute, date.second);
		st7735_write_string(0, 20, str, &font_ascii_8x16, ST7735_BLUE, ST7735_BLACK);

		if (!weather_ok || t % 60 == 0)
		{
			const char *rsp;
			esp_at_http_get(weather_uri, &rsp, NULL, 10000);
			weather_t weather;
			weather_parse(rsp, &weather);

			snprintf(str, sizeof(str), "%s", "Ithaca");
			st7735_write_string(0, 64, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
			snprintf(str, sizeof(str), "%s", weather.weather);
			st7735_write_string(0, 64, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
			snprintf(str, sizeof(str), "%s C", weather.temperature);
			st7735_write_string(0, 64, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
		}

		if (!sntp_ok || t % 3600 == 0)
		{
			uint32_t ts;
			sntp_ok = esp_at_sntp_init();
			delay_ms(1000);
			esp_at_time_get(&ts);
			rtc_set_timestamp(ts - 4 * 60 * 60);
		}
	}
}
