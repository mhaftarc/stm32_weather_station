#include "timer_app.h"
#include "tim.h"
#include <stdbool.h>

volatile bool sensor_read_flag = false;


void timer_app_start(void){

	HAL_TIM_Base_Start_IT(&htim2);

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance == TIM2){
	sensor_read_flag = true;
	}
}
