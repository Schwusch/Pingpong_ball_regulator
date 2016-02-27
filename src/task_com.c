/*
 * task_com.c
 *
 * Created: 2016-02-19 14:35:52
 *  Author: Jonathan
 */ 
#include <asf.h>
#include "sync.h"

void task_com(void *pvParameters)
{	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = timer;
	
	xLastWakeTime = xTaskGetTickCount();
	uint8_t str[BUF_LEN] = {0};
	
	while(1)
	{	
		/*	Set pin high for performance measurement	 */
		ioport_set_pin_level(PIO_PB26_IDX,HIGH);
		
		if(xSemaphoreTake(sync, portMAX_DELAY)){
			itoa(meas_distance, str, 10);
			printf(str);
			printf("\n");
			
			itoa(err, str, 10);
			printf(str);
			printf("\n");
			
			itoa(output, str, 10);
			printf(str);
			printf("\n");
			xSemaphoreGive(sync);
		}
		/*	Set pin low for performance measurement	 */
		ioport_set_pin_level(PIO_PB26_IDX,LOW);
		
		/* Sleep for some time */
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
		
		
	}
}