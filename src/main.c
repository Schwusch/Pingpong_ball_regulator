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
uint16_t timer = 50;
uint16_t dac_value = 666;
uint16_t adc_distance[CONV_ARR_LENGTH] = {0};
uint16_t *p_adc_distance = adc_distance;

static void configure_console(void)
/* Enables feedback through the USB-cable back to terminal within Atmel Studio */
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
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
