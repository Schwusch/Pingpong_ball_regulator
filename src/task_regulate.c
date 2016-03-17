/*
 * task_regulate.c
 *
 * Created: 2016-02-19 15:03:14
 *  Author: Jonathan
 */ 
#include <asf.h>
#include "pwm_func.h"
#include "sync.h"
#include "fdacoefs.h"

void task_regulate(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = timer;
	xLastWakeTime = xTaskGetTickCount();
	
	int invalue = 0;
	int int_sum = 0;
	int old_error = 0;
	int new_error = 0;
	int delta_error = 0;
	int calc_output = 0;
	int calc_distance = 0;
	float dT = (float) timer/1000;
	
	int xbuff[BL] = {0};
	
	while(1){
		/* Set pin high for performance measurement */
		ioport_set_pin_level(PIO_PB14_IDX,HIGH);
		
		/*Start ADC and read the value */
		adc_start(ADC);
		while ((adc_get_status(ADC) & 0x1<<24) == 0);
		invalue = adc_get_latest_value(ADC);
		
		/* Move invalue buffer one sample forward */
		
		for(int k = BL - 1; k > 0; k--){
			xbuff[k] = xbuff[k-1];
		}
		xbuff[0] = invalue;
		
		/* filter the signal */
		float temp_sum = 0;
		for(int k = 0; k < BL; k++){
			temp_sum += xbuff[k] * B[k];
		}
		invalue = (int) temp_sum;
		
		/* Calculate ball distance */
		uint8_t adc_to_mm_index = min(max((invalue/10) - 1, 0), 98);
		uint8_t diff = adc_to_mm[adc_to_mm_index] - adc_to_mm[adc_to_mm_index + 1];
		uint8_t interpol = (diff * (invalue % 10)) / 10;		
		calc_distance = adc_to_mm[adc_to_mm_index] + interpol;
		
		/* Control calculation */
		new_error = set_point - calc_distance;
		int_sum += new_error;
		
		/* Limit integral sum */
		int_sum = max(min(int_sum, antiwindup), -antiwindup);
		
		delta_error = old_error - new_error;	
		float p_part = (float) (new_error * (-prop_gain));
		float i_part = (float) ((dT * int_sum)/int_gain) * (-prop_gain);
		float d_part = (float) ((delta_error/dT) * der_gain) * (-prop_gain);
		float sum = p_part + i_part + d_part;
		calc_output = (int) (offset + sum);
		
		old_error = new_error;		
		/* Limit output to 0-100% */
		calc_output = max(min(calc_output, 999), 0);
		
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

void regulate_init(void)
{
	pwm_set_duty_cycle(offset);
	delay_ms(5000);
}
