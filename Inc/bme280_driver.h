#ifndef BME280_DRIVER_H_
#define BME280_DRIVER_H_
#include <stdbool.h>


#define BME280_I2C_ADRESS (0x76 << 1)
#define BME280_REG_ID      0xD0
#define BME280_CHIP_ID     0x60


extern bme280_calibration_data_t bme280_calibration_set;


typedef struct {
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
	uint8_t dig_H1;
	int16_t dig_H2;
	uint8_t dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;

} bme280_calibration_data_t;

bool bme280_connected(void);
void bme280_read_calibration(void);
#endif
