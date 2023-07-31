 /******************************************************************************
 *
 * Module: Application
 *
 * File Name: application.c
 *
 * Description: application to measure the distance using ultrasonic sensor
 *
 * Author: Aly Maher
 *
 *******************************************************************************/

#include "../HAL/lcd.h"
#include <avr/io.h>
#include "../HAL/ultrasonic.h"

#define F_CPU 8000000
uint16 reading=0;

int main(void)
{
	SREG|=(1<<7);
	LCD_init();
	LCD_displayString("Distance=");
	Ultrasonic_init();

while(1)
	{
		reading=Ultrasonic_readDistance();
		LCD_moveCursor(0,11);
		LCD_intgerToString(reading);
		LCD_displayString("cm    ");

	}


}
