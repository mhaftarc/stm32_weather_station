#include "sensor.h"
#include "bme280_driver.h"
#include "i2c.h"

#include <stdbool.h>
#include <stddef.h>

#define SENSOR_MAX_ERRORS   3

static uint8_t error_count = 0;
static bool sensor_ready = false;


static HAL_StatusTypeDef sensor_recover(void)
{
	HAL_StatusTypeDef status;

	status = HAL_I2C_DeInit(&hi2c1);
	if (status != HAL_OK) {
		return status;
	}

	status = HAL_I2C_Init(&hi2c1);
	if (status != HAL_OK) {
		return status;
	}

	return bme280_init();
}

HAL_StatusTypeDef sensor_init(void)
{
	HAL_StatusTypeDef status = bme280_init();

	error_count = 0;
	sensor_ready = (status == HAL_OK);

	return status;
}

HAL_StatusTypeDef sensor_read(SensorData *data)
{
	HAL_StatusTypeDef status;

	if (data == NULL) {
		return HAL_ERROR;
	}

	if (!sensor_ready) {
		status = sensor_recover();
		if (status != HAL_OK) {
			return status;
		}
		sensor_ready = true;
		error_count = 0;
	}

	status = bme280_read_data();
	if (status != HAL_OK) {
		error_count++;
		if (error_count >= SENSOR_MAX_ERRORS) {
			sensor_ready = false;
			error_count = 0;
		}
		return status;
	}

	error_count = 0;

	data->temperature = (float)bme280_temperature / 100.0f;
	data->pressure    = (float)bme280_pressure / 25600.0f;
	data->humidity    = (float)bme280_humidity / 1024.0f;

	return HAL_OK;
}
