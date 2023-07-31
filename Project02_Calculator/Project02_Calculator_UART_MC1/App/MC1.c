/*
 * app.c
 *
 *  Created on: Jun 22, 2023
 *      Author: Aly Maher
 */


#include "..\MCAL\gpio.h"
#include "..\MCAL\uart.h"
#include "..\HAL\keypad.h"
#include "..\LIBS\Types.h"
#include "..\LIBS\Utils.h"
#include <util/delay.h> /* For the delay functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main()
{
	UART_init(9600);
	char key;

	while(1)
	{
		key = keypad_read(); // Read key from keypad
		UART_sendByte(key);
		_delay_ms(500);
		
		
	}


}


