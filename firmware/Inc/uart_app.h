#ifndef UART_APP_H_
#define UART_APP_H_
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "sensor.h"




void uart_app_send_sensor_data(HAL_StatusTypeDef status, const SensorData *data);

#endif
