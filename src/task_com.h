/*
 * task_com.h
 *
 * Created: 2016-02-19 14:50:39
 *  Author: Jonathan
 */ 


#ifndef TASK_COM_H_
#define TASK_COM_H_

#define TASK_COM_STACKSIZE	(2048/sizeof(portSTACK_TYPE))
#define NEWLINE				10

void task_com(void *pvParameters);


#endif /* TASK_COM_H_ */