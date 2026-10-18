#include "lcd_driver.h"

bool lcd_connected(void)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_IsDeviceReady(&hi2c3, LCD_I2C_ADDRESS, 2, 10);

    return (status == HAL_OK);
}





static HAL_StatusTypeDef lcd_write_nibble(uint8_t nibble, uint8_t rs){
	uint8_t buf[2];
	uint8_t base;
	HAL_StatusTypeDef status;



	base = (nibble << 4) | rs | LCD_BACKLIGHT;  // no rw since we write all the time
	buf[0] = base | LCD_EN;
	buf[1] = base;
	status = HAL_I2C_Master_Transmit(&hi2c3, LCD_I2C_ADDRESS, buf, 2, 100);
	return status;
}



static HAL_StatusTypeDef lcd_send_byte(uint8_t value, uint8_t rs)
{
	HAL_StatusTypeDef status = lcd_write_nibble((value >> 4), rs);
	if (status != HAL_OK) {
	    return status;
	}
	status = lcd_write_nibble((value & 0x0F), rs);
	return status;

}

HAL_StatusTypeDef lcd_command(uint8_t command)
{
	return lcd_send_byte(command, 0);
}

HAL_StatusTypeDef lcd_letter(char c)
{
	return lcd_send_byte((uint8_t)c, LCD_RS);
}

HAL_StatusTypeDef lcd_init(void)
{
    if (!lcd_connected()) {
        return HAL_ERROR;
    }
//initialization process from the data sheet
    HAL_Delay(50);
    lcd_write_nibble(0x3, 0);
    HAL_Delay(5);
    lcd_write_nibble(0x3, 0);
    HAL_Delay(1);
    lcd_write_nibble(0x3, 0);
    HAL_Delay(1);
    lcd_write_nibble(0x2, 0); // 4 bit mode
    HAL_Delay(1);

    lcd_command(LCD_CMD_FUNCTION_SET);
    lcd_command(LCD_CMD_DISPLAY_OFF);
    lcd_command(LCD_CMD_CLEAR);
    HAL_Delay(2);
    lcd_command(LCD_CMD_ENTRY_MODE);
    lcd_command(LCD_CMD_DISPLAY_ON);


    return HAL_OK;
}


HAL_StatusTypeDef lcd_print(const char *str)
{
    HAL_StatusTypeDef status;
	uint8_t i = 0;
    while (str[i] != '\0') {
    	status = lcd_letter(str[i]);
    	if (status != HAL_OK) {
    	    return status;
    	 }
    	i++;
    }
    return HAL_OK;
}
