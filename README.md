# STM32 Weather Clock

This project is a compact, Wi-Fi-enabled weather and temperature display system built with an STM32F103 ("Blue Pill") microcontroller, WIFI board (Raspberrypi Pico W & ESP32-C3 used in this project), and an ST7735 SPI LCD. It displays real-time weather conditions, temperature, and time using data pulled from the internet, alongside live ambient temperature sensing and automatic screen rotation based on tilt.

## Features

-  **Live weather data** via OpenWeatherMap API (using ESP32/Pico W + Wi-Fi)
-  **Real-time clock** synchronized from the internet (UTC)
-  **Local temperature sensing** using a thermistor (NTC) and ADC
-  **Automatic screen rotation** based on MPU6050 accelerometer (I2C)
-  **Graphical output** using ST7735 LCD and DMA-accelerated SPI
-  **Serial communication** between ESP32/Pico W and STM32 via USART

## Hardware

| Component                       | Description                              |
|---------------------------------|------------------------------------------|
| STM32F103C8T6                   | Main microcontroller (Blue Pill)         |
| Raspberry Pi Pico W / ESP32-C3  | Wi-Fi-enabled weather data fetcher       |
| ST7735 LCD                      | 128x128 SPI color display                |
| MPU6050                         | Accelerometer/gyroscope (I2C)            |
| NTC Thermistor                  | Analog temperature sensor                |


## How It Works

1. **Pico W / ESP32** connects to Wi-Fi and fetches weather + time from [OpenWeatherMap] via code/AT command(https://openweathermap.org).
2. The wifi board sends data via **USART** to the STM32.
3. **STM32F103** parses and processes the data and updates the LCD via **DMA-based SPI**.
4. A **thermistor** connected to ADC provides local temperature data.
5. The **MPU6050 sensor** detects tilt and triggers screen rotation dynamically.

## Directory Guide
1. The ESP32 version is included in folder **/demo**
2. The Pico W version is included in folder **/stm32_project_final**
3. the rest of the files include single functionality, which could be used in this project or other projects, the functionality is as in the name of the folders

## How to use (pico w version):

### 1. Flash the Pico W

- Upload `weather_sender.py` to the Pico W using Thonny or rshell
- Replace placeholders with your Wi-Fi credentials and API key

### 2. Flash the STM32

- Compile using Keil, STM32CubeIDE, or makefile toolchain
- Wire UART, I2C, ADC, and SPI pins correctly to peripherals

### 3. Power On and Test

- The LCD should display current weather, time, and ambient temperature
- Tilting the device should rotate the display automatically

## How to use (ESP32-C3 version):

### 1. Flash the STM32

- Compile using Keil, STM32CubeIDE, or makefile toolchain
- Wire UART, I2C, ADC, and SPI pins correctly to peripherals

### 3. Power On both STM and ESP and Test

- The LCD should display current weather, time, and ambient temperature
- Tilting the device should rotate the display automatically

