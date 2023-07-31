/*
 * Timer.c
 *
 * Created: 10/7/2021 11:34:11 AM
 * Author : Marco
 */ 

#define F_CPU 1000000UL

#include "MCAL/Timers.h"
#include "MCAL/gpio.h"
#include <util/delay.h>

//extern uint8_t u8_TOVNumber ;
uint8_t tick;
uint8_t count_second_flag ;
uint8_t seconds_count;
uint8_t minutes_count;
uint8_t hours_count;
uint8_t pause ;

ISR(INT0_vect)
{
	seconds_count = 0;
	minutes_count = 0;
	hours_count = 0;
}
ISR(INT1_vect)
{
	while((GPIO_readPin(PORTB_ID,PIN2_ID)));
}
ISR(INT2_vect)
{
}
void INT0_Init(void)
{
	SREG  &= ~(1<<7);      // Disable interrupts by clearing I-bit
	// Enable external interrupt pin INT0
	GICR  |= (1<<INT0);
	// Trigger INT0 with the falling edge
	MCUCR &= ~(1<<ISC00);
	MCUCR |= (1<<ISC01);
	SREG  |= (1<<7);       // Enable interrupts by setting I-bit
}
void INT1_Init(void)
{
	SREG  &= ~(1<<7);      // Disable interrupts by clearing I-bit
	// Enable external interrupt pin INT1
	GICR|=(1<<INT1);
	// Trigger INT1 with the rising edge
	MCUCR |= (1<<ISC10);
	MCUCR |= (1<<ISC11);
	SREG  |= (1<<7);       // Enable interrupts by setting I-bit
}
void INT2_Init(void)
{
	SREG  &= ~(1<<7);      // Disable interrupts by clearing I-bit
	// Trigger INT2 with the falling edge
	GICR|=(1<<INT2);
	MCUCSR&=~(1<<ISC2);
	SREG  |= (1<<7);       // Enable interrupts by setting I-bit

}
void OCIE_Callback()
{
	/* timer interrupt will be called every 250 ms */
	tick++;
	if (tick==4)
	{
		count_second_flag=1;
		tick=0;
	}
}

int main(void)
{
	GPIO_setupPortDirection(PORTA_ID, PORT_OUTPUT);
	GPIO_setupPortDirection(PORTC_ID, PORT_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN2_ID, PIN_INPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN2_ID, PIN_INPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN3_ID, PIN_INPUT);
	// enable internal pull up resistor at INT0/PD2 pin
	PORTD |= (1<<PD2);
	// enable internal pull up resistor at INT0/PD2 pin
	PORTB |= (1<<PB2);
	// initialize all the 7-segment with zero value
	//SREG  |= (1<<7);       // Enable interrupts by setting I-bit
	PORTA |= 0x3F;
	PORTC &= 0xF0;
	sei();
	STIMER0_CONFIG config = {CTC, OCIE_ENABLE, TOIE_DISABLE, PRESCALING_CLK1024};
	TIMER0_Init(&config);
	TIMER0_CALLBACK_CompareMatch_INTERRUPT(OCIE_Callback);
	TIMER0_SetCounterValue(0);  /* initiate timer counter */
	TIMER0_SetCompareValue(250); /* put compare value in OCR0=250 */
	INT0_Init();
	INT1_Init();
	INT2_Init();

	while (1)
	{
		if(count_second_flag == 1)
		{
			//enter here every one second
			//increment seconds count
			seconds_count++;
			if(seconds_count == 60)
			{
				seconds_count = 0;
				minutes_count++;
			}
			if(minutes_count == 60)
			{
				minutes_count = 0;
				hours_count++;
			}
			if(hours_count == 24)
			{
				hours_count = 0;
			}
			// reset the flag again
			count_second_flag = 0;
		}
		else
		{
			// out the number of seconds
			PORTA = (PORTA & 0xC0) | 0x01;
			PORTC = (PORTC & 0xF0) | (seconds_count%10);

			// make small delay to see the changes in the 7-segment
			// 2Miliseconds delay will not effect the seconds count
			_delay_ms(2);

			PORTA = (PORTA & 0xC0) | 0x02;
			PORTC = (PORTC & 0xF0) | (seconds_count/10);

			_delay_ms(2);

			// out the number of minutes
			PORTA = (PORTA & 0xC0) | 0x04;
			PORTC = (PORTC & 0xF0) | (minutes_count%10);

			_delay_ms(2);

			PORTA = (PORTA & 0xC0) | 0x08;
			PORTC = (PORTC & 0xF0) | (minutes_count/10);

			_delay_ms(2);

			// out the number of hours
			PORTA = (PORTA & 0xC0) | 0x10;
			PORTC = (PORTC & 0xF0) | (hours_count%10);

			_delay_ms(2);

			PORTA = (PORTA & 0xC0) | 0x20;
			PORTC = (PORTC & 0xF0) | (hours_count/10);

			_delay_ms(2);
		}
	}
}

