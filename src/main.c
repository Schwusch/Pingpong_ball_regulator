/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "motorshield.h"
#include "adc_func.h"
#include "sync.h"
#include "pwm_func.h"
#include "io_uart.h"
#include "task_com.h"
#include "task_regulate.h"

//xSemaphoreHandle sync = 1;

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	ioport_init();
	
	motorshield_init();	
	adc_config();
	uart_config(115200);
	pwm_config();
	//vSemaphoreCreateBinary(sync);
	

	
	
	xTaskCreate(task_com, (const signed char * const) "Com", TASK_COM_STACKSIZE, NULL, 2, NULL);
	

	
	if(xTaskCreate(task_regulate, (const signed char * const) "Regulate", TASK_COM_STACKSIZE,NULL,2,NULL) !=pdPASS)
	{
		//Shouldn't happen
	}
	
	vTaskStartScheduler();
	
	/*
	int flag1 = 0;
	int str[20] = {0};
	int *p_str = str;
	
	
	while(1)
	{
		//TODO: Lots of stuff
		if(uart_receiver_ready() && flag1==0){
			*p_str = uart_receive_char();
			if(*p_str == 10){
				flag1 = 1;
				p_str = str;
			} else {
				p_str++;
			}
		}
		if(uart_transmitter_ready() && flag1 ){
			uart_send_char(*p_str);
			if (*p_str == 10)
			{
				flag1 = 0;
				p_str = str;
			} else {
				p_str++;
			}
		}
	}
	*/
	
}
