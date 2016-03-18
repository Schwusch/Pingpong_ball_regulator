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
#include "task_regulate.h"

/* This task reads an analog value, filters it 
 * and calculates PID value for fan PWM 
 */
void task_regulate(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = timer;
	xLastWakeTime = xTaskGetTickCount();
	
	while(1){		
		/*Start ADC and read the value */
		adc_start(ADC);
		while ((adc_get_status(ADC) & 0x1<<24) == 0);		
		int invalue = adc_get_latest_value(ADC);		
		/* filter the signal */
		invalue = signal_filter(invalue);		
		/* Calculate ball distance */
		int calc_distance = calculate_distance(invalue);		
		/* Control calculation */
		int calc_output = pid_controller(calc_distance);		
		/* Write output */
		pwm_set_duty_cycle(calc_output);
		/* Sleep for some time */
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}
/* Sets a value (offset) to PWM duty cycle and waits a 
 * couple of seconds so that the ball will be in measurable range 
 * by the time the controller task starts
 */
void regulate_init(void)
{
	pwm_set_duty_cycle(offset);
	delay_ms(FAN_INIT_MS);
}

/* Filters the signal with a FIR filter 
 * designed with Matlabs fdatool 
 */
int signal_filter(int invalue)
{
	static int xbuff[BL] = {0};
	float temp_sum = 0;
		
	/* Move invalue buffer one sample forward */	
	for(int k = BL - 1; k > 0; k--){
		xbuff[k] = xbuff[k-1];
	}
	xbuff[0] = invalue;
	
	/* filter the signal */	
	for(int k = 0; k < BL; k++){
		temp_sum += xbuff[k] * B[k];
	}	
	return (int) temp_sum;
}
/* Converts the AD signal to a 
 * distance in mm from the sensor 
 */
int calculate_distance(int invalue)
{
	/* The index of the array is the AD-value divided by 10 */
	/* The index is limited to array boundaries */
	int adc_to_mm_index = min(max((invalue/10) - 1, 0), CONV_ARR_LENGTH - 2);
	/* An interpolation is calculated to achieve an as correct value as possible */
	int diff = adc_to_mm[adc_to_mm_index] - adc_to_mm[adc_to_mm_index + 1];
	int interpol = (diff * (invalue % 10)) / 10;
	
	return (adc_to_mm[adc_to_mm_index] + interpol);
}

/* Calculates a PWM value with a PID algorithm
 * Then copies process variables to global variables
 * with a thread safe semaphore handling
 */
int pid_controller(int calc_distance)
{
	/* Recursive error sum */
	static int int_sum = 0;	
	/* Error since last function call */
	static int old_error = 0;
	/* Time between executions in seconds */
	const float dT = (float) timer/1000;
	/* Error is difference between set point and measured distance */
	int new_error = set_point - calc_distance;
	/* Error is added to error sum */
	int_sum += new_error;
	
	/* Limit integral sum */
	int_sum = max(min(int_sum, antiwindup), -antiwindup);	
	int delta_error = old_error - new_error;
	/* PID algorithm is based on Ziegler-Nichols implementation */
	float p_part = (float) (new_error * (-prop_gain));
	float i_part = (float) ((dT * int_sum)/int_gain) * (-prop_gain);
	float d_part = (float) ((delta_error/dT) * der_gain) * (-prop_gain);
	float sum = p_part + i_part + d_part;
	/* An offset is added to the PID sum */
	int calc_output = (int) (offset + sum);
	/* Limit output to 0-100% */
	calc_output = max(min(calc_output, 999), 0);
	old_error = new_error;	
	/* Transfer values between tasks */
	if(xSemaphoreTake(sync, portMAX_DELAY)){
		meas_distance = calc_distance;
		err = new_error;
		output = calc_output;
		xSemaphoreGive(sync);
	}			
	return calc_output;
}