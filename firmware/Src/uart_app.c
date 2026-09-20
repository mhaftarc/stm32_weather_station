#include "uart_app.h"
#include "sensor.h"
#include "main.h"




void uart_app_send_sensor_data(void){
	char buffer[100];
	SensorData test_data = mock_sensor();
	snprintf(buffer, sizeof(buffer), "Temperature: %.1f C | Humidity: %.1f %% | Pressure: %1.f hPa\r\n",
			test_data.temperature,
			test_data.humidity,
			test_data.pressure);
	HAL_UART_Transmit(&huart2, 	(uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);



}
