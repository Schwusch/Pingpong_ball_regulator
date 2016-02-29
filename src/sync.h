/*
 * sync.h
 *
 * Created: 2016-02-19 14:46:06
 *  Author: Jonathan
 */ 


#ifndef SYNC_H_
#define SYNC_H_

extern xSemaphoreHandle sync;
extern uint16_t adc_to_mm[];
extern uint16_t meas_distance;
extern uint16_t err;
extern uint16_t output;
extern uint16_t timer;
extern float prop_gain;
extern float int_gain;
extern float der_gain;
extern uint16_t set_point;

#define CONV_ARR_LENGTH 100
#define BUF_LEN 16

#endif /* SYNC_H_ */