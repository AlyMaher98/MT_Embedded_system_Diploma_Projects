/*
 * app.c
 *
 *  Created on: Jun 22, 2023
 *      Author: Aly Maher
 */



#include "..\MCAL\gpio.h"
#include "..\MCAL\uart.h"
#include "..\HAL\lcd.h"
#include "..\LIBS\Types.h"
#include "..\LIBS\Utils.h"
#include <util/delay.h> /* For the delay functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t flag;
uint32_t multiplier;
uint8_t operator;
uint8_t operator_index=0;
uint32_t answer;
uint32_t num1_adjusted;
uint32_t num2_adjusted;
uint8_t press_after_answer;

int performOperation(uint32_t num1, uint32_t num2, uint8_t operator)
{

	uint32_t result=0;
	switch (operator)
	{
	case '+':
		result = num1 + num2;

		break;
	case '-':
		result =  num1 - num2;

		break;
	case '*':
		result =  num1 * num2;

		break;
	case '%':
		result =  num1 / num2;

		break;
	default:
		return 0;
	}
	return result;
}

int main()
{
	LCD_init();
	UART_init(9600);


	uint8_t key_recieved;

	while(1)
	{
		/* Receive the pressed key from MC1 through UART */
		key_recieved = UART_recieveByte();
		_delay_ms(100); /* Press time */
		if (key_recieved == '=')
		{
			answer=performOperation(num1_adjusted,num2_adjusted,operator);
			LCD_moveCursor(1,0);
			LCD_displayString("Result:");
			LCD_intgerToString(answer); // Display the result
			press_after_answer++;

		}
		else if ( (key_recieved >=0) && (key_recieved <= 9) )
		{

				if (operator_index==0)
				{

					LCD_intgerToString(key_recieved); /* display the pressed keypad switch */


					if (flag==0)
					{
						num1_adjusted=key_recieved;
						flag++;
					}
					else
						num1_adjusted = (num1_adjusted*10)+key_recieved;


					_delay_ms(100); /* Press time */
				}
				else
				{
					LCD_intgerToString(key_recieved); /* display the pressed keypad switch */
					if (flag==0)
					{
						num2_adjusted=key_recieved;
						flag++;
					}
					else
						num2_adjusted = (num2_adjusted*10)+key_recieved;

				}
				_delay_ms(100); /* Press time */

			}


		else if  (key_recieved == '+' || key_recieved == '-' || key_recieved == '*' || key_recieved == '%')
		{
			if (press_after_answer==0)
			{operator_index=1;
			operator=key_recieved;
			LCD_displayCharacter(operator); /* display the pressed keypad switch */
			_delay_ms(100); /* Press time */
			flag=0;
			}
			else
			{
				LCD_clearScreen();
				operator=key_recieved;
				LCD_intgerToString(answer);
				LCD_displayCharacter(operator);
				num1_adjusted=answer;
				num2_adjusted=0;
				operator_index=1;


			}
		}
		else if (key_recieved==13)
		{
			LCD_clearScreen();
			num1_adjusted=0;
			num2_adjusted=0;
			operator_index=0;
			answer=0;
			flag=0;
			press_after_answer=0;
			_delay_ms(100); /* Press time */




		}
	}


}


