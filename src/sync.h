/*
 * sync.h
 *
 * Created: 2016-02-19 14:46:06
 *  Author: Jonathan
 */ 


#ifndef SYNC_H_
#define SYNC_H_

extern xSemaphoreHandle sync;
extern uint16_t dac_value;
extern uint16_t *p_adc_distance;
extern uint16_t timer;
#define CONV_ARR_LENGTH 100
#define NEWLINE 10



#endif /* SYNC_H_ */