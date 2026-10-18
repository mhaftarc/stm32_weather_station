#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "i2c.h"

#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H
#define LCD_I2C_ADDRESS   (0x27 << 1) // the adress of the lcd
#define LCD_RS          (1 << 0) // 0 for command, 1 for letter
#define LCD_EN           (1 << 2) // it is the enable read = 1
#define LCD_RW          (1 << 1)   // it is write = 0, or read = 1
#define LCD_BACKLIGHT   (1 << 3)   // the lighting on the screen

// bits 4-7 are data bits, we set then as a variable nibble

// function adresses from the datasheet
#define LCD_CMD_CLEAR        0x01
#define LCD_CMD_ENTRY_MODE   0x06
#define LCD_CMD_DISPLAY_OFF  0x08
#define LCD_CMD_DISPLAY_ON   0x0C
#define LCD_CMD_FUNCTION_SET 0x28
bool lcd_connected(void);


HAL_StatusTypeDef lcd_init(void);
HAL_StatusTypeDef lcd_command(uint8_t command);
HAL_StatusTypeDef lcd_letter(char c);








#endif
