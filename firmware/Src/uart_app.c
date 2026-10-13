#include "uart_app.h"
#include "sensor.h"
#include "main.h"
#include "bme280_driver.h"




void uart_app_send_sensor_data(void){
    char buffer[100];
    HAL_StatusTypeDef status;
    SensorData data;

    status = sensor_read(&data);

    if(status != HAL_OK){
    	snprintf(buffer, sizeof(buffer), "ERR,%d\r\n", (int)status);
  	  BSP_LED_On(LED2);

    }else{
    snprintf(buffer, sizeof(buffer), "%.2f,%.2f,%.2f\r\n",
            data.temperature,
            data.pressure,
            data.humidity);
    BSP_LED_Off(LED2);
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

}
