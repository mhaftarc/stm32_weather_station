#include "bme280_driver.h"




bool bme280_connected(void){
	uint8_t memory_state;
	HAL_I2C_Mem_Read(&hi2c1,BME280_I2C_ADDR, BME280_REG_ID, I2C_MEMADD_SIZE_8BIT, &memory_state, 1, 100);
	if(memory_state == BME280_CHIP_ID){
		return true;
	}else{
		return false;
	}
}
