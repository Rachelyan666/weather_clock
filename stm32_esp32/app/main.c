#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "timer.h"
#include "rtc.h"
#include "st7735.h"
#include "stfonts.h"
#include "stimage.h"
#include "lcd_spi.h"

static void on_timer_elapsed(){
	static uint32_t counter = 0;
	
	if (counter == 500){
		counter = 0;
		led_toggle();
	}
	counter++;
}

int main(void){

	board_init();
	led_init();
	timer_init(1000);
	timer_elapsed_register(on_timer_elapsed);
	timer_start();
	st7735_init();

    st7735_fill_screen(ST7735_BLACK);

	rtc_init();
	rtc_date_t date = {2025, 7, 19, 17, 58, 24};
	rtc_set_date(&date);

    char str[64];
    while (1)
    {
		delay_ms(100);
		rtc_get_date(&date);

        sprintf(str, "Year: %u", date.year);
        st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
		sprintf(str, "Month: %u", date.month);
        st7735_write_string(0, 20, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
		sprintf(str, "Day: %u", date.day);
        st7735_write_string(0, 40, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
		sprintf(str, "Hour: %u", date.hour);
        st7735_write_string(0, 60, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
		sprintf(str, "Minute: %u", date.minute);
        st7735_write_string(0, 80, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
		sprintf(str, "Second: %u", date.second);
        st7735_write_string(0, 100, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
    }
}
