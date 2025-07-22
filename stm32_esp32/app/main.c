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

	char line[128];
	rtc_date_t date;

	while (1)
	{
		// --- Display RTC time at top ---
		rtc_get_date(&date);
		snprintf(str, sizeof(str), "%04d-%02d-%02d", date.year, date.month, date.day);
		st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
		snprintf(str, sizeof(str), "%02d:%02d:%02d", date.hour, date.minute, date.second);
		st7735_write_string(0, 16, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);

		// --- Wait for new line from Pico W ---
		if (receive_line(line, sizeof(line)))
		{
			// Format: Ithaca,Clear,12.6,2025-07-22,03:53:17
			char city[32], weather_str[32], temp_str[16], date_str[16], time_str[16];

			if (sscanf(line, "%31[^,],%31[^,],%15[^,],%15[^,],%15s",
								 city, weather_str, temp_str, date_str, time_str) == 5)
			{
				// --- Display city, weather, temp ---
				st7735_write_string(0, 64, city, &font_ascii_8x16, ST7735_BLUE, ST7735_BLACK);
				st7735_write_string(0, 80, weather_str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
				st7735_write_string(0, 96, temp_str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);

				// --- Convert to rtc_date_t ---
				sscanf(date_str, "%4d-%2d-%2d", &date.year, &date.month, &date.day);
				sscanf(time_str, "%2d:%2d:%2d", &date.hour, &date.minute, &date.second);

				// --- Update RTC ---
				rtc_set_date(&date);
			}
		}

		delay_ms(500); // Update screen & poll rate
	}
}
