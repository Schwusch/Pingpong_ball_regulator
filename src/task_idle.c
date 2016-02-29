/*
 * task_idle.c
 *
 * Created: 2/24/2016 14:42:23
 *  Author: Jonathan
 */ 
#include <asf.h>
#include "task_idle.h"

void task_idle(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 50;
	
	xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}	
}