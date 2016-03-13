/*
 * uart_recieve_values.c
 *
 * Created: 2/24/2016 15:45:26
 *  Author: Jonathan
 */ 

#include <asf.h>
#include "sync.h"
#include "uart_recieve_values.h"

void matlab_recieve_values(void)
{
	/* Buffer for receiving a string */
	uint8_t buffer[BUF_LEN] = {0};
	
	/* Receive the conversion array */
	for (int i = 0; i < CONV_ARR_LENGTH; i++)
	{
		gets(buffer);
		adc_to_mm[i] = atoi(buffer);
	}		
	gets(buffer);
	prop_gain = (float)atoi(buffer) / 1000;	
	gets(buffer);
	int_gain = (float)atoi(buffer) / 1000;	
	gets(buffer);
	der_gain = (float)atoi(buffer) / 1000;	
	gets(buffer);
	set_point = atoi(buffer);	
	gets(buffer);
	offset = atoi(buffer);	
	gets(buffer);
	antiwindup = atoi(buffer);	
	gets(buffer);
	timer = atoi(buffer);	
}

void configure_console(void)
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
