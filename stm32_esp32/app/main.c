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
#include "esp_usart.h"
#include "weather.h"

// pin connection
// esp32 gpio 6 -> stm32 A2
// esp32 gpio 7 -> stm32 A3
// gnd to gnd

// UART receive buffer
static char rx_buffer[128];
static uint16_t rx_len = 0;
static bool line_ready = false;

// UART receive callback
static void usart_received(uint8_t data) {
    if (line_ready) return;  // skip if previous line not yet processed

    if (data == '\n') {
        rx_buffer[rx_len] = '\0';  // null terminate
        line_ready = true;
    } else if (rx_len < sizeof(rx_buffer) - 1) {
        rx_buffer[rx_len++] = data;
    }
}

int main(void)
{

	board_init();
	led_init();
	rtc_init();
	esp_usart_init();

	st7735_init();
	st7735_fill_screen(ST7735_BLACK);
	
	esp_usart_receive_register(usart_received);

	char str[64];
    rtc_date_t date;

    while (1) {
        // Display current RTC time
        rtc_get_date(&date);
        snprintf(str, sizeof(str), "%04d-%02d-%02d", date.year, date.month, date.day);
        st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
        snprintf(str, sizeof(str), "%02d:%02d:%02d", date.hour, date.minute, date.second);
        st7735_write_string(0, 16, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);

        // Process full line when available
        if (line_ready) {
            char city[32] = {0}, weather_str[32] = {0}, temp_str[16] = {0};
            char date_str[16] = {0}, time_str[16] = {0};

            char *token;
            token = strtok(rx_buffer, ",");
            if (token) strncpy(city, token, sizeof(city));
            token = strtok(NULL, ",");
            if (token) strncpy(weather_str, token, sizeof(weather_str));
            token = strtok(NULL, ",");
            if (token) strncpy(temp_str, token, sizeof(temp_str));
            token = strtok(NULL, ",");
            if (token) strncpy(date_str, token, sizeof(date_str));
            token = strtok(NULL, ",");
            if (token) strncpy(time_str, token, sizeof(time_str));

            // Display received weather info
            st7735_fill_rect(0, 64, 160, 48, ST7735_BLACK);
            st7735_write_string(0, 64, city, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
            st7735_write_string(0, 80, weather_str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
            st7735_write_string(0, 96, temp_str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);

            // Parse and set RTC
            int y, m, d, h, min, s;
            sscanf(date_str, "%4d-%2d-%2d", &y, &m, &d);
            sscanf(time_str, "%2d:%2d:%2d", &h, &min, &s);

            date.year = y;
            date.month = m;
            date.day = d;
            date.hour = h;
            date.minute = min;
            date.second = s;
            rtc_set_date(&date);

            // Reset buffer
            rx_len = 0;
            line_ready = false;
        }

        delay_ms(500);  // Limit screen updates
    }
}
