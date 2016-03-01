/*
 * Author: Jonathan
 */
#include <asf.h>

#include "motorshield.h"
#include "adc_func.h"
#include "sync.h"
#include "pwm_func.h"
#include "task_com.h"
#include "task_regulate.h"
#include "task_idle.h"
#include "uart_recieve_values.h"

xSemaphoreHandle sync = 1;

uint16_t adc_to_mm[CONV_ARR_LENGTH] = {0};
uint16_t meas_distance = 0;
int err = 0;
uint16_t output = 0;
uint16_t timer;
float prop_gain;
float int_gain;
float der_gain;
uint16_t set_point;

int main (void)
{
	sysclk_init();
	board_init();
	configure_console();
	ioport_init();	
	motorshield_init();	
	adc_config();
	pwm_config();
	
	ioport_set_pin_dir(PIO_PB26_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(PIO_PB14_IDX, IOPORT_DIR_OUTPUT);
	
	matlab_recieve_values();
	
	vSemaphoreCreateBinary(sync);
	xTaskCreate(task_idle, (const signed char * const) "Idle", TASK_COM_STACKSIZE, NULL, 1, NULL);	
	xTaskCreate(task_com, (const signed char * const) "Com", TASK_COM_STACKSIZE, NULL, 2, NULL);	
	xTaskCreate(task_regulate, (const signed char * const) "Regulate", TASK_COM_STACKSIZE,NULL,2,NULL);
	
	vTaskStartScheduler();	
}
