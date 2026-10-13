#include "uart_app.h"
#include "sensor.h"
#include "main.h"
#include "bme280_driver.h"




void uart_app_send_sensor_data(void){
    char buffer[100];
    bme280_read_data();
    snprintf(buffer, sizeof(buffer), "%.2f,%.2f,%.2f\r\n",
            (float)bme280_temperature / 100.0f,
            (float)bme280_pressure / 25600.0f,
            (float)bme280_humidity / 1024.0f);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
