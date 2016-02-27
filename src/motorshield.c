/*
 * motorshield.c
 *
 * Created: 2016-02-19 13:59:23
 *  Author: Jonathan
 */ 

#include <asf.h>
#include "motorshield.h"

void motorshield_init(void)
{
	ioport_set_pin_dir(CHECK_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(PWM_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PWM_PIN, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_dir(DIRECTION_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(DIRECTION_PIN, IOPORT_PIN_LEVEL_HIGH);
}