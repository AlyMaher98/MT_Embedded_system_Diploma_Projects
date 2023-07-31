/*
 * Timers.h
 *
 * Created: 10/7/2021 11:39:09 AM
 *  Author: Aly Maher
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_


/******************************************************************************
*                               INCLUDES			                          *
*******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../LIBS/Types.h"
#include "../LIBS/Utils.h"



void Timer0_Fast_PWM(uint8_t speed);

#endif /* TIMERS_H_ */
