/*
 * uart_recieve_values.c
 *
 * Created: 2/24/2016 15:45:26
 *  Author: jonathan
 */ 

#include <asf.h>
#include "sync.h"
#include "uart_recieve_values.h"
#include "io_uart.h"
#define BUF_LEN 16

void matlab_recieve_values(void)
{
	char buffer[BUF_LEN] = {0};
	
	for (int i = 0; i < CONV_ARR_LENGTH; i++)
	{
		uart_receive_string(buffer, BUF_LEN);
		*(p_adc_distance + i) = atoi(buffer);
	}
	
	/* send back the array to make sure its alright */
	for (int i = 0; i < CONV_ARR_LENGTH; i++)
	{
		itoa(*(p_adc_distance + i), buffer, 10);
		uart_send_string(buffer);
		uart_send_newline();
	}
}
