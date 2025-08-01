#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "rtc.h"
#include "timer.h"
#include "mpu6050.h"
#include "st7735.h"
#include "stfonts.h"
#include "stimage.h"
#include "weather.h"
#include "ntc.h"
#include "esp_usart.h"

// UART receive buffer
static char rx_buffer[128];
static uint16_t rx_len = 0;
static bool line_ready = false;

static uint32_t runms;
static uint32_t disp_height;

static void timer_elapsed_callback(void)
{
    runms++;
    if (runms >= 24 * 60 * 60 * 1000)
    {
        runms = 0;
    }
}

// UART receive callback
static void usart_received(uint8_t data)
{
    if (line_ready)
        return; // skip if previous line not yet processed

    if (data == '\n')
    {
        rx_buffer[rx_len] = '\0'; // null terminate
        line_ready = true;
    }
    else if (rx_len < sizeof(rx_buffer) - 1)
    {
        rx_buffer[rx_len++] = data;
    }
}

static void redraw_screen(const rtc_date_t *date, const char *weather_str, const char *temp_str)
{
    char str[64];

    // Date
    snprintf(str, sizeof(str), "%02d-%02d-%02d", date->year % 100, date->month, date->day);
    st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);

    // Time
    snprintf(str, sizeof(str), "%02d:%02d", date->hour, date->minute);
    st7735_write_string(0, 70, str, &font_time_24x48, ST7735_CYAN, ST7735_BLACK);

    // Weather icon or description
    const st_image_t *img = NULL;
    if (strcmp(weather_str, "Clear") == 0) {
        img = &icon_weather_qing;
    } else if (strcmp(weather_str, "Rain") == 0 || strcmp(weather_str, "Drizzle") == 0) {
        img = &icon_weather_yu;
    }

    if (img != NULL) {
        st7735_draw_image(0, 20, img->width, img->height, img->data);
    } else {
        snprintf(str, sizeof(str), "%s", weather_str);
        st7735_write_string(0, 16, str, &font_ascii_8x16, ST7735_CYAN, ST7735_BLACK);
    }

    // Weather temperature
    snprintf(str, sizeof(str), "%s", temp_str);
    st7735_write_string(78, 0, str, &font_temper_16x32, ST7735_BLUE, ST7735_BLACK);

    // Internal temperature (MPU)
    float temper = mpu6050_read_temper();
    snprintf(str, sizeof(str), "%5.1fC", temper);
    st7735_write_string(78, 40, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
}

int main(void)
{

    board_init();

    led_init();
    rtc_init();
    timer_init(1000);
    timer_elapsed_register(timer_elapsed_callback);
    timer_start();
    mpu6050_init();
    ntc_init();

    st7735_init();
    st7735_fill_screen(ST7735_BLACK);

    // boot window
    st7735_write_string(0, 0, "Initializing...", &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
    disp_height += font_ascii_8x16.height;
    delay_ms(2000);

    esp_usart_init();

    st7735_write_string(0, disp_height, "Ready", &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
    disp_height += font_ascii_8x16.height;
    delay_ms(500);

    st7735_fill_screen(ST7735_BLACK);

    esp_usart_receive_register(usart_received);

    runms = 0;
    uint32_t last_runms = runms;
    char str[64];
	char str2[64];
    rtc_date_t date;
	int last_orientation = -1;
    static char last_weather[32] = "Clear";
    static char last_temp[16] = "20.0";

    while (1)
    {

        if (runms == last_runms)
        {
            continue;
        }
        last_runms = runms;

        int orientation = detect_orientation();
        if (orientation >= 0 && orientation != last_orientation)
        {
            st7735_set_rotation(orientation);
            st7735_fill_screen(ST7735_BLACK);
            last_orientation = orientation;

            rtc_get_date(&date);  // update latest date
            redraw_screen(&date, last_weather, last_temp);
        }


        if (last_runms % 100 == 0)
        {
            // Display current RTC time
            rtc_get_date(&date);
            snprintf(str, sizeof(str), "%02d-%02d-%02d", date.year % 100, date.month, date.day);
            st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
			snprintf(str, sizeof(str), "%02d%s%02d", date.hour, date.second % 2 ? ":" : ":", date.minute);
            st7735_write_string(0, 70, str, &font_time_24x48, ST7735_CYAN, ST7735_BLACK);
        }
        // Process full line when available
        if (line_ready || last_runms % (10 * 60 * 1000) == 0)
        {
            char city[32] = {0}, weather_str[32] = {0}, temp_str[16] = {0};
            char date_str[16] = {0}, time_str[16] = {0};

            char *token;
            token = strtok(rx_buffer, ",");
            if (token)
                strncpy(city, token, sizeof(city));
            token = strtok(NULL, ",");
            if (token)
                strncpy(weather_str, token, sizeof(weather_str));
                strncpy(last_weather, weather_str, sizeof(last_weather));
            token = strtok(NULL, ",");
            if (token)
                strncpy(temp_str, token, sizeof(temp_str));
                strncpy(last_temp, temp_str, sizeof(last_temp));
            token = strtok(NULL, ",");
            if (token)
                strncpy(date_str, token, sizeof(date_str));
            token = strtok(NULL, ",");
            if (token)
                strncpy(time_str, token, sizeof(time_str));

            const st_image_t *img = NULL;
            if (strcmp(weather_str, "Clouds") == 0)
            {
               img = &icon_weather_duoyun;
            }
            else if (strcmp(weather_str, "Clear") == 0)
            {
                img = &icon_weather_qing;
            }
            else if (strcmp(weather_str, "Rain") == 0 || strcmp(weather_str, "Drizzle") == 0)
            {
                img = &icon_weather_yu;
            }

            if (img != NULL)
            {
                st7735_draw_image(0, 20, img->width, img->height, img->data);
            }
            else
            {
                snprintf(str, sizeof(str), "%s", weather_str);
                st7735_write_string(0, 16, str, &font_ascii_8x16, ST7735_YELLOW, ST7735_BLACK);
            }

            snprintf(str, sizeof(str), "%s", temp_str);
            st7735_write_string(78, 0, str, &font_temper_16x32, ST7735_BLUE, ST7735_BLACK);

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
				
				
	    if (last_runms % (100) == 0)
            {
							
               // uint16_t ntc = ntc_read();
                //int temp_c = adc_to_temp(ntc);
                //snprintf(str2, sizeof(str2), "%dC", temp2);
                float temper = mpu6050_read_temper();
                snprintf(str, sizeof(str), "%5.1fC", temper);
                st7735_write_string(78, 40, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
            }

        delay_ms(500); // Limit screen updates
    }
}
