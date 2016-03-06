/*
 * task_regulate.c
 *
 * Created: 2016-02-19 15:03:14
 *  Author: Jonathan
 */ 
#include <asf.h>
#include "pwm_func.h"
#include "sync.h"

#define MOV_AVER_LENGTH 20
#define OFFSET 410

void task_regulate(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = timer;
	xLastWakeTime = xTaskGetTickCount();
	
	uint16_t invalue = 0;
	int int_sum = 0;
	int old_error = 0;
	int new_error = 0;
	int delta_error = 0;
	int calc_output = 0;
	uint16_t calc_distance = 0;
	
	static uint16_t xbuff[MOV_AVER_LENGTH] = {0};
	
	while(1){
		/* Set pin high for performance measurement */
		ioport_set_pin_level(PIO_PB14_IDX,HIGH);
		
		/*Start ADC and read the value */
		adc_start(ADC);
		while ((adc_get_status(ADC) & 0x1<<24) == 0);
		invalue = adc_get_latest_value(ADC);
		
		/* Move moving average buffer one sample forward */
		
		for(int k = MOV_AVER_LENGTH - 1; k > 0; k--){
			xbuff[k] = xbuff[k-1];
		}
		xbuff[0] = invalue;
		
		/* Calculate the moving average */
		uint16_t temp_sum = 0;
		for(int k = 0; k < MOV_AVER_LENGTH; k++){
			temp_sum += xbuff[k];
		}
		invalue = temp_sum / MOV_AVER_LENGTH;
		
		/* Calculate ball distance */
		uint8_t adc_to_mm_index = (invalue/10) - 1;
		uint8_t diff = adc_to_mm[adc_to_mm_index] - adc_to_mm[adc_to_mm_index + 1];
		uint8_t interpol = (diff * (invalue % 10)) / 10;		
		calc_distance = adc_to_mm[adc_to_mm_index] + interpol;
		
		/* Control calculation */
		new_error = set_point - calc_distance;
		int_sum += new_error;
		
		/* Limit integral sum */
		if(int_sum > 2500){	int_sum = 2500;	} 
		else if (int_sum < -2500){ int_sum = -2500;	}
		
		delta_error = old_error - new_error;		
		calc_output = (int) (OFFSET + (-prop_gain) * (new_error + (timer * int_sum)/int_gain + (delta_error/timer) * der_gain));
		
		old_error = new_error;
		
		/* Limit output to 0-100% */
		if (calc_output > 999){ calc_output = 999; }
		else if(calc_output < 0){ calc_output = 0; }
		
		/* Write output */
		pwm_set_duty_cycle(calc_output);
		
		/* Transfer values between tasks */
		if(xSemaphoreTake(sync, portMAX_DELAY)){
			meas_distance = calc_distance;
			err = new_error;
			output = calc_output;
			xSemaphoreGive(sync);
		}
		
		/* Set pin low for performance measurement */
		ioport_set_pin_level(PIO_PB14_IDX,LOW);
		
		/* Sleep for some time */
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}