# STM32 Weather Station

Small weather station on the Nucleo-F446RE. I wanted to practice I2C, UART and timers, and write my own drivers instead of using ready libraries.

A BME280 measures temperature, pressure and humidity. The values are shown on a 16x2 LCD and sent over UART to my PC, where a Python script plots them live and saves them to CSV.

## What it does

- Reads the sensor every 1 s
- Shows the values on the LCD:

```text
T: 24.5C H: 55%
P: 1012.3hPa
```

- Sends the same values over UART:

```text
24.50,1012.30,55.10
```

- If the sensor fails, the LCD shows `SENSOR ERROR`, UART sends `ERR,<code>` and LD2 turns on
- You can unplug the sensor and plug it back in, it starts working again without a reset

## What I used

- STM32 Nucleo-F446RE
- C
- STM32CubeIDE + HAL
- I2C (two buses)
- UART
- TIM2 interrupt
- BME280
- LCD 1602 with PCF8574 backpack
- Python (pyserial, matplotlib)
- Git

## Wiring

| What | Pins | Notes |
|---|---|---|
| BME280 | I2C1: PB6 (SCL), PB7 (SDA) | 3.3 V, addr 0x76 |
| LCD + PCF8574 | I2C3: PA8 (SCL), PC9 (SDA) | 5 V, addr 0x27 |
| UART | USART2: PA2/PA3 | through ST-LINK, 115200 |
| LED | LD2 (PA5) | on when sensor fails |

## Project structure

```text
firmware/Src/
├── bme280_driver.c
├── sensor.c
├── lcd_driver.c
├── lcd_app.c
├── uart_app.c
├── timer_app.c
└── main.c
python_logger/
└── logger.py
```

I split the code into drivers (talking to the hardware) and app files (what to do with the data). The LCD driver just prints text and doesn't know anything about the sensor, so I can use it in other projects.

## How it works

TIM2 runs at 84 MHz with PSC = 8399 and ARR = 9999, which gives an interrupt every 1 s. The interrupt only sets a flag, and everything else happens in the main loop, so nothing slow runs inside the interrupt.

The main loop reads the sensor once and sends the same data to UART and the LCD:

```c
sensor_status = sensor_read(&data);
uart_app_send_sensor_data(sensor_status, &data);
lcd_app_show_sensor_data(sensor_status, &data);
```

Timestamps are added in Python, not on the STM.

## Notes

I wrote the BME280 driver myself instead of using the Bosch library, because I wanted to see how the calibration and compensation actually work.

All driver functions return a HAL status so errors don't get lost. After 3 failed reads in a row I reset I2C1 and init the sensor again.

The LCD is on a separate I2C bus, because the PCF8574 board has pull-ups to 5 V and the BME280 can't take 5 V. PA8 and PC9 are 5 V tolerant so the STM is fine with it.

The LCD runs in 4-bit mode, because the PCF8574 only has 8 pins and 4 of them are used for control (RS, RW, EN, backlight).
