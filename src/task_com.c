/*
 * task_com.c
 *
 * Created: 2016-02-19 14:35:52
 *  Author: Dennis, Jonathan
 */ 
#include <asf.h>
#include "io_uart.h"
#include "sync.h"

void task_com(void *pvParameters)
{	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 50;
	
	xLastWakeTime = xTaskGetTickCount();

	int timer = 10;
	
	static int flag1 = 0;
	static uint8_t str[20] = {0};
	static uint8_t *p_str = str;
	
	while(1)
	{		
		//TODO: Lots of stuff
		while (flag1==0)
		{
			while(!uart_receiver_ready());
			*p_str = uart_receive_char();
			if(*p_str != 10){
				p_str++;
			} else {
				flag1 = 1;
				p_str = str;
			}
		}
		while(flag1==1){
			while(!uart_transmitter_ready());
			uart_send_char(*p_str);
			if ((char)*p_str == 10)
			{
				flag1 = 0;
				p_str = str;
				} else {
				p_str++;
			}			
		}
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}