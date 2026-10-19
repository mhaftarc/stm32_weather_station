#include "lcd_app.h"


void lcd_app_show_sensor_data(HAL_StatusTypeDef status, const SensorData *data){

	char line [17]; // its 17 since line has 16 colummns plus \0


	if(status != HAL_OK){
		lcd_set_cursor(0, 0);
		lcd_print("  SENSOR ERROR  ");
		lcd_set_cursor(1, 0);
		lcd_print("   GODDAMN IT   ");
		return;
	}


	snprintf(line, sizeof(line), "T:%5.1fC H:%3.0f%% ", data->temperature, data->humidity);
	lcd_set_cursor(0, 0);
	lcd_print(line);


	snprintf(line, sizeof(line), "P:%7.1fhPa    ", data->pressure);
	lcd_set_cursor(1, 0);
	lcd_print(line);


}
