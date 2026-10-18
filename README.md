# stm32_weather_station

Weather station on Nucleo-F446RE. BME280 measures temperature, pressure and humidity,
data goes over UART to my PC where a Python script plots it live. There is also a 16x2 LCD.

STM32CubeIDE + HAL. I wrote the BME280 and LCD drivers myself.

## Wiring

- BME280 -> I2C1 (PB6 SCL, PB7 SDA), 3.3 V, addr 0x76
- LCD 1602 + PCF8574 -> I2C3 (PA8 SCL, PC9 SDA), 5 V, addr 0x27
- UART2 (PA2/PA3) through ST-LINK, 115200
- LD2 lights up when the sensor fails

## How it works

TIM2 interrupt every 1 s (PSC 8399, ARR 9999 at 84 MHz). The interrupt only sets a flag,
the actual reading and sending is done in the main loop. Output format is `temp,press,hum`
or `ERR,<code>` if something went wrong. Timestamps are added in Python.

## Some decisions

- My own BME280 driver instead of the Bosch library, because I wanted to understand
  the calibration and compensation part.
- All driver functions return HAL status, so errors don't get lost.
- If the sensor fails 3 times in a row, I2C1 gets reset and the BME280 is initialized again.
  You can unplug and plug it back in and it starts working again.
- LCD is on a separate bus, because the PCF8574 board has pull-ups to 5 V and BME280
  can't take 5 V on its pins. PA8 and PC9 are 5 V tolerant so the STM is fine.
- LCD works in 4-bit mode (PCF8574 has only 8 pins, 4 of them are control signals).

## Problems I had

- TIM2 was firing way too often and flooded the UART, had to fix PSC/ARR.
- Reading calibration data overflowed my buffer.
- Uninitialized buffer + ignored return values = random readings.
- Pressure plot looked like noise because of matplotlib axis offset.
- LCD backlight didn't work, turned out the jumper was missing.
