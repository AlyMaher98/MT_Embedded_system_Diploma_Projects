/*
 * Timers.c
 *
 * Created: 10/7/2021 11:39:21 AM
 *  Author: Marco
 */ 

#include "pwm.h"

#include "..\MCAL\gpio.h"



void Timer0_Fast_PWM(uint8_t speed)
{

	TCNT0 = 0; //Set Timer Initial value
	OCR0  =  (short)(((float)speed/100)*255 + 0.5); // Set Compare Value
	GPIO_setupPinDirection(PORTB_ID, PIN3, PIN_OUTPUT);
	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */

	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);

}
