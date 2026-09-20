#include "sensor.h"




SensorData mock_sensor(void){
	SensorData data;

	data.temperature = 23;
	data.humidity = 76;
	data.pressure = 1013;

	return data;
}
