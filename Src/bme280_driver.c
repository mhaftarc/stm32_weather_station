#include "bme280_driver.h"
#include "i2c.h"

bme280_calibration_data_t bme280_calib;
int32_t t_fine;

bool bme280_connected(void){
	uint8_t memory_state;
	HAL_I2C_Mem_Read(&hi2c1,BME280_I2C_ADRESS, BME280_REG_ID, I2C_MEMADD_SIZE_8BIT, &memory_state, 1, 100);
	if(memory_state == BME280_CHIP_ID){
		return true;
	}else{
		return false;
	}
}

void bme280_read_calibration(void){
	uint8_t calib_data_1[26];
	uint8_t calib_data_2[7];
	HAL_I2C_Mem_Read(&hi2c1,BME280_I2C_ADRESS, 0x88, I2C_MEMADD_SIZE_8BIT, calib_data_1, 26, 100);
	HAL_I2C_Mem_Read(&hi2c1,BME280_I2C_ADRESS, 0xE1, I2C_MEMADD_SIZE_8BIT, calib_data_2, 16, 100);


	bme280_calib.dig_T1 = calib_data_1[0] | (calib_data_1[1] << 8);
	bme280_calib.dig_T2 = calib_data_1[2] | (calib_data_1[3] << 8);
	bme280_calib.dig_T3 = calib_data_1[4] | (calib_data_1[5] << 8);
	bme280_calib.dig_P1 = calib_data_1[6] | (calib_data_1[7] << 8);
	bme280_calib.dig_P2 = calib_data_1[8] | (calib_data_1[9] << 8);
	bme280_calib.dig_P3 = calib_data_1[10] | (calib_data_1[11] << 8);
	bme280_calib.dig_P4 = calib_data_1[12] | (calib_data_1[13] << 8);
	bme280_calib.dig_P5 = calib_data_1[14] | (calib_data_1[15] << 8);
	bme280_calib.dig_P6 = calib_data_1[16] | (calib_data_1[17] << 8);
	bme280_calib.dig_P7 = calib_data_1[18] | (calib_data_1[19] << 8);
	bme280_calib.dig_P8 = calib_data_1[20] | (calib_data_1[21] << 8);
	bme280_calib.dig_P9 = calib_data_1[22] | (calib_data_1[23] << 8);
	bme280_calib.dig_H1 = calib_data_1[25];
	bme280_calib.dig_H2 = calib_data_2[0] | (calib_data_2[1] << 8);
	bme280_calib.dig_H3 = calib_data_2[2];
	bme280_calib.dig_H4 = (calib_data_2[3] << 4) | (calib_data_2[4] & 0x0F);
	bme280_calib.dig_H5 = (calib_data_2[5] << 4) | (calib_data_2[4] >> 4);
	bme280_calib.dig_H6 = calib_data_2[6];
}


void bme280_configure(void){
	uint8_t ctrl_hum_value = 0x01; // oversampling x1 humidity
	uint8_t ctrl_meas_value = ((0x01 << 5) | (0x01 << 2) | (0x3)); // oversampling x1 of temperature and pressure, set to normal working mode

	HAL_I2C_Mem_Write(&hi2c1, BME280_I2C_ADRESS, 0xF2, I2C_MEMADD_SIZE_8BIT, &ctrl_hum_value,sizeof(ctrl_hum_value),100);
	HAL_I2C_Mem_Write(&hi2c1, BME280_I2C_ADRESS, 0xF4, I2C_MEMADD_SIZE_8BIT, &ctrl_meas_value,sizeof(ctrl_meas_value),100);
}

void bme280_read_data(void){
	uint8_t measurement[8];
	HAL_I2C_Mem_Read(&hi2c1, BME280_I2C_ADRESS, 0xF7, I2C_MEMADD_SIZE_8BIT, measurement, sizeof(measurement),100);


	int32_t adc_P = (measurement[0] << 12 | measurement[1] << 4 | measurement[2] >> 4);
	int32_t adc_T = (measurement[3] << 12 | measurement[4] << 4 | measurement[5] >> 4);
	int32_t adc_H = (measurement[6] << 8 | measurement[7]);

	bme280_temperature = bme280_compensate_T(adc_T);
	bme280_pressure = bme280_compensate_P(adc_P);
	bme280_humidity = bme280_compensate_H(adc_H);

}




// these 3 functions are copied from bosch documentation as they recommend to use their api, or use these functions on our own responsibility

int32_t bme280_compensate_T(int32_t adc_T)
{
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)bme280_calib.dig_T1 << 1))) * ((int32_t)bme280_calib.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)bme280_calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)bme280_calib.dig_T1))) >> 12) * ((int32_t)bme280_calib.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

uint32_t bme280_compensate_P(int32_t adc_P)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bme280_calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)bme280_calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)bme280_calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bme280_calib.dig_P3) >> 8) + ((var1 * (int64_t)bme280_calib.dig_P2) << 12);
    var1 = ((((int64_t)1 << 47) + var1)) * ((int64_t)bme280_calib.dig_P1) >> 33;
    if (var1 == 0)
    {
        return 0;
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bme280_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)bme280_calib.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)bme280_calib.dig_P7) << 4);
    return (uint32_t)p;
}

uint32_t bme280_compensate_H(int32_t adc_H)
{
    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)bme280_calib.dig_H4) << 20) - (((int32_t)bme280_calib.dig_H5) *
                 v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r *
                 ((int32_t)bme280_calib.dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)bme280_calib.dig_H3)) >> 11) +
                 ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)bme280_calib.dig_H2) +
                 8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                ((int32_t)bme280_calib.dig_H1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    return (uint32_t)(v_x1_u32r >> 12);
}
