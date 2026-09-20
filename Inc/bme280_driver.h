#ifndef BME280_DRIVER_H_
#define BME280_DRIVER_H_
#include <stdbool.h>
#include <stdint.h>

#define BME280_I2C_ADRESS (0x76 << 1)
#define BME280_REG_ID      0xD0
#define BME280_CHIP_ID     0x60


extern int32_t bme280_temperature;
extern uint32_t bme280_pressure;
extern uint32_t bme280_humidity;


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
	int8_t dig_H6;

} bme280_calibration_data_t;

extern bme280_calibration_data_t bme280_calibration_set;


bool bme280_connected(void);
void bme280_read_calibration(void);
void bme280_configure(void);
void bme280_read_data(void);
int32_t bme280_compensate_T(int32_t adc_T);
uint32_t bme280_compensate_P(int32_t adc_P);
uint32_t bme280_compensate_H(int32_t adc_H);




#endif
