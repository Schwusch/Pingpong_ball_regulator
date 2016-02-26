/*
 * task_regulate.c
 *
 * Created: 2016-02-19 15:03:14
 *  Author: Dennis, Jonathan
 */ 

#include <asf.h>
#include "pwm_func.h"
#include "io_uart.h"
#include "sync.h"

void task_regulate(void *pvParameters)
{
	uint16_t invalue;
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = timer;
	xLastWakeTime = xTaskGetTickCount();
	
	static float xbuff[4] = {0};
	int k;
	float temp = 0;
	
	while(1){
		ioport_set_pin_level(PIO_PB14_IDX,HIGH);
		adc_start(ADC);
		while ((adc_get_status(ADC) & 0x1<<24)==0);
		invalue = adc_get_latest_value(ADC);
		
		if(xSemaphoreTake(sync, portMAX_DELAY)){
			dac_value = invalue;
			xSemaphoreGive(sync);
		}
		
		for(k = 3; k > 0; k--){
			xbuff[k] = xbuff[k-1];
		}
		xbuff[0] = invalue;
		
		for(k = 0; k <= 3; k++){
			temp += 0.25*xbuff[k];
		}
		invalue = (uint16_t)temp;
		
		pwm_set_duty_cycle(invalue);
		ioport_set_pin_level(PIO_PB14_IDX,LOW);
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}