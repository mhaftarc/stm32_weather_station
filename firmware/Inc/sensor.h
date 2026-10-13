
#ifndef SENSOR_H_
#define SENSOR_H_
#include "stm32f4xx_hal.h"


typedef struct {
	float temperature;
	float humidity;
	float pressure;
} SensorData;


HAL_StatusTypeDef sensor_init(void);
HAL_StatusTypeDef sensor_read(SensorData *data);



#endif
