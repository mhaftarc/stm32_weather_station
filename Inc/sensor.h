
#ifndef SENSOR_H_
#define SENSOR_H_

typedef struct {
	float temperature;
	float humidity;
	float pressure;
} SensorData;


SensorData mock_sensor(void);




#endif
