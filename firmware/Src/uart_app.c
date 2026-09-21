#include "uart_app.h"
#include "sensor.h"
#include "main.h"




void uart_app_send_sensor_data(void){
	char buffer[100];
	SensorData test_data = mock_sensor();
	snprintf(buffer, sizeof(buffer), "%.2f,%.2f,%.2f\r\n",
			test_data.temperature,
			test_data.pressure,
			test_data.humidity);
	HAL_UART_Transmit(&huart2, 	(uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);



}
