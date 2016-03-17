/*
 * task_regulate.h
 *
 * Created: 2/19/2016 15:58:42
 *  Author: Jonathan
 */ 


#ifndef TASK_REGULATE_H_
#define TASK_REGULATE_H_

void task_regulate(void *pvParameters);
void regulate_init(void);
int signal_filter(int);
int calculate_distance(int);
int pid_controller(int);

#endif /* TASK_REGULATE_H_ */