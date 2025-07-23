#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "main.h"
#include "led.h"
#include "usart.h"
#include "ntc.h"
#include "st7735.h"
#include "stfonts.h"
#include <math.h>

static uint8_t data_buffer[16];
static uint16_t data_len;

#define BETA      3950.0f       // Beta coefficient
#define R_25      10000.0f      // Thermistor resistance at 25�C
#define R_FIXED   10000.0f      // Series resistor (10k typical)
#define ADC_MAX   4095.0f       // 12-bit ADC
#define T0_KELVIN 298.15f       // 25�C in Kelvin

float adc_to_temp(uint16_t adc_value)
{
    float voltage_ratio = adc_value / ADC_MAX;
    float resistance = R_FIXED * (voltage_ratio / (1.0f - voltage_ratio));

    float temp_k = 1.0f / (1.0f / T0_KELVIN + logf(resistance / R_25) / BETA);
    float temp_c = temp_k - 273.15f;
    return temp_c;
}


int main(void)
{

	board_init();

	usart_init();
	ntc_init();
	st7735_init();
  st7735_fill_screen(ST7735_GREEN);
	
	st7735_write_string(0, 0, "Initializing...", &font_ascii_8x16, ST7735_WHITE, ST7735_BLACK);
	delay_ms(1000);

	char str2[64];
	while (true)
	{
		delay_ms(100);
		uint16_t ntc = ntc_read();
							
        int temp_c = adc_to_temp(ntc);
        snprintf(str2, sizeof(str2), "%dC", temp_c);
                // float temper = mpu6050_read_temper();
                // snprintf(str, sizeof(str), "%5.1fC", temper);
        st7735_write_string(78, 40, str2, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
	}
}
