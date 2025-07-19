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
#include "mpu6050.h"


int main(void)
{
    board_init();

    led_init();
    usart_init();
    st7735_init();
    mpu6050_init();

    st7735_fill_screen(ST7735_BLACK);

    char str[64];
    while (1)
    {
        delay(20);
        mpu6050_accel_t accel;
        mpu6050_read_accel(&accel);


        mpu6050_gyro_t gyro;
        mpu6050_read_gyro(&gyro);


        float temp = mpu6050_read_temper();


        sprintf(str, "Accel: %.2f, %.2f, %.2f\r\n", accel.x, accel.y, accel.z);
        usart_write_string(str, false);
        sprintf(str, "Accel:");
        st7735_write_string(0, 0, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
        sprintf(str, "%6.2f", accel.x);
        st7735_write_string(0, 16, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
        sprintf(str, "%6.2f", accel.y);
        st7735_write_string(0, 32, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
        sprintf(str, "%6.2f", accel.z);
        st7735_write_string(0, 48, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);


        sprintf(str, "gyro: %.2f, %.2f, %.2f\r\n", gyro.x, gyro.y, gyro.z);
        usart_write_string(str, false);
        sprintf(str, "Gyro:");
        st7735_write_string(64, 0, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
        sprintf(str, "%7.2f", gyro.x);
        st7735_write_string(64, 16, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
        sprintf(str, "%7.2f", gyro.y);
        st7735_write_string(64, 32, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
        sprintf(str, "%7.2f", gyro.z);
        st7735_write_string(64, 48, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);


        sprintf(str, "temper: %.2f\r\n", temp);
        usart_write_string(str, false);
        sprintf(str, "Temper:");
        st7735_write_string(0, 128 - 16, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);
        sprintf(str, "%4.1f", temp);
        st7735_write_string(64, 128 - 16, str, &font_ascii_8x16, ST7735_GREEN, ST7735_BLACK);

    }
}
