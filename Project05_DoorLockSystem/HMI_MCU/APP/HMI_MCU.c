/*
 * HMI_MCU.c
 *
 *  Created on: Jun 28, 2023
 *      Author: Aly Maher
 */

#include "HMI_MCU.h"


int main()
{
	uint8 key = 0, receivedByte = 0;
	LCD_init();
	UART_init(9600);
	SET_BIT(SREG, PIN7_ID);     /* Enable Global Interrupt (I-bit) */
	/* Timer Configuration:
	 * Timer ID --> Timer 1
	 * Timer Mode --> CTC Mode
	 * Initial Value --> 0
	 * Timer_Prescaler --> FCPU/1024
	 * Compare Value --> 7813
	 * AS FCPU = 8MHz so F_timer = 8MHz/1024 = 128us & To force timer to produce interrupt every 1 second
	 * SO Compare Value = 1/128us = 7813
	 */
	Timer_ConfigType config = { TIMER1, COMPARE_MODE, 0, 7813, FCPU_1024, DUMMY };
	Timer_setCallBack(HMI_timerCallBack,TIMER1);
	Timer_init(&config);
	g_Password_Match_Status = PASSWORD_UNMATCHED;      /* Initial value of the password status as UNMATCHED */
	HMI_SystemPasswordInit(g_InputPassword);

	while(1)
	{
		while(1)
		{
			LCD_MainMenu();           /* Displaying the main options to the user */

			key = keypad_read();   /* Taking input from the keypad */

			if (key == '+')
			{
				LCD_WritePass();
				HMI_getPassword(g_InputPassword);          /* Get password from user and store it in global array */
				UART_sendByte(READY_TO_SEND);              /* inform Control ECU to start sending */
				HMI_sendPasswordByUART(g_InputPassword);   /* Sending that password to Control ECU */

				UART_sendByte(OPEN_DOOR_OPTION);           /* Inform Control ECU that User chose Open Door Option */

				/* Control ECU responses [either the password is correct or wrong] */
				receivedByte = UART_recieveByte();
				if (receivedByte == UNLOCKING_DOOR)
				{
					HMI_OpenDoor();                /* Start displaying door status on LCD */
					g_wrongPasswordCounter = 0;    /* Reset the counter */
				}
				else if (receivedByte == WRONG_PASSWORD)
				{
					LCD_clearScreen();
					LCD_displayString("Wrong Password");

					g_wrongPasswordCounter++;       /* Increment the counter */

					if (g_wrongPasswordCounter == NUMBER_OF_WRONG_PASSWORD_ATTEMPTS)
					{
						/* System should be locked no inputs from Keypad will be accepted during this time period */
						g_sec = 0;
						while (g_sec < KEYPAD_LOCKED_PERIOD);
					}
					else
					{
						/* Small delay to see "Wrong Password" message on the LCD */
						_delay_ms(MESSAGE_DISPLAY_DELAY);
					}
				}
			}

			else if(key == '-')
			{
				LCD_WritePass();
				HMI_getPassword(g_InputPassword);          /* Get password from user and store it in global array */
				UART_sendByte(READY_TO_SEND);              /* inform Control ECU to start sending */
				HMI_sendPasswordByUART(g_InputPassword);   /* Sending that password to Control ECU */

				UART_sendByte(CHANGE_PASSWORD_OPTION);     /* inform Control ECU that user chose Change Password Option */

				/* Control ECU responses [either the password is correct or wrong] */
				receivedByte = UART_recieveByte();

				/* If user enters the old password right, then let user create a new system password*/
				if (receivedByte == CHANGING_PASSWORD)
				{
					HMI_SystemPasswordInit(g_InputPassword);
					LCD_clearScreen();
					g_wrongPasswordCounter = 0;      /* reset the counter */
				}
				else if (receivedByte == WRONG_PASSWORD)
				{
					LCD_clearScreen();
					LCD_displayString("Wrong Password");

					g_wrongPasswordCounter++;       /* Increment the counter */

					if (g_wrongPasswordCounter == NUMBER_OF_WRONG_PASSWORD_ATTEMPTS)
					{
						/* System should be locked no inputs from Keypad will be accepted during this time period */
						g_sec = 0;
						while (g_sec < KEYPAD_LOCKED_PERIOD);
					}
					else
					{
						/* Small delay to see "Wrong Password" message on the LCD */
						_delay_ms(MESSAGE_DISPLAY_DELAY);
					}
				}
			}
		}
	}
}
void HMI_SystemPasswordInit(uint8 *password)
{
	while(g_Password_Match_Status == PASSWORD_UNMATCHED)
	{
		/* Entering the password for the first time */
		LCD_WritePass();
		HMI_getPassword(password);

		/* Sending the password to the Control ECU By UART */
		UART_sendByte(READY_TO_SEND);
		while(UART_recieveByte() != READY_TO_RECEIVE);
		HMI_sendPasswordByUART(g_InputPassword);

		/* Entering the confirmation password */
		LCD_ReWritePass();
		HMI_getPassword(password);

		/* Sending the Confirmation password to the Control ECU By UART */
		UART_sendByte(READY_TO_SEND);
		while(UART_recieveByte() != READY_TO_RECEIVE);
		HMI_sendPasswordByUART(g_InputPassword);

		/* Receiving Passwords Match Status from Control ECU By UART */
		while(UART_recieveByte() != READY_TO_SEND);
		UART_sendByte(READY_TO_RECEIVE);
		g_Password_Match_Status = UART_recieveByte();

		/* Checking on the Matching Status */
		if (g_Password_Match_Status == PASSWORD_MATCHED)
			break;
		else
		{
			LCD_clearScreen();
			LCD_displayString("PASSWORD MISMATCH");
			_delay_ms(MESSAGE_DISPLAY_DELAY);
		}
	}
	g_Password_Match_Status = PASSWORD_UNMATCHED;
}

void HMI_getPassword(uint8 *password)
{
	uint8 i = 0, key = 0;

	/* Taking password of length = 5 from the keypad */
	while(i != PASSWORD_LENGTH)
	{
		key = keypad_read();

		if ((key >= 1) && (key <= 9))
		{
			password[i] = key;
		 	LCD_displayCharacter('*');        /* Display '*' on LCD for each number */
			i++;
		}
		/* Delay between each input taken from the keypad */
		_delay_ms(KEYPAD_INPUT_DELAY);
	}

	/* Loop until user presses the ENTER key from the keypad */
	while(keypad_read() != ENTER_KEY_PRESSED);
}
/*
 * Description: Function to send the entered password Byte by Byte to the Control ECU By UART
 */
void HMI_sendPasswordByUART(uint8 *password)
{
	uint8 count = 0;

	while(count != PASSWORD_LENGTH)
	{
		UART_sendByte(password[count]);
		count++;
		_delay_ms(100);
	}
}

/*
 * Description: A function that displays on LCD that door is opening or closing for a certain period of time
 */
void HMI_OpenDoor(void)
{
	/* Hold the system for 15 seconds & display to user that door is unlocking */
	g_sec = 0;
	LCD_Door_unlockPrint();
	while (g_sec < DOOR_UNLOCKING_PERIOD);

	/* Hold the door open for 3 seconds */
	g_sec = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is now open");
	while (g_sec < DOOR_LEFT_OPEN_PERIOD);

	/* Hold the system for 15 seconds & display to user that door is locking */
	g_sec = 0;
	LCD_Door_lockPrint();
	while (g_sec < DOOR_LOCKING_PERIOD);
}

void LCD_WritePass(void)
{
	LCD_clearScreen();
	LCD_displayString("Enter Password:");
	LCD_moveCursor(1,0);
}
void LCD_ReWritePass(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Re-Enter the same");
	LCD_displayStringRowColumn(1, 0, "password: ");
	LCD_moveCursor(1, 10);
}
void LCD_Astricsprint(void)
{
	LCD_displayString("*");

}

void LCD_WriteNewPass(void)
{
	LCD_clearScreen();
	LCD_displayString("Enter New Pass:");
	LCD_moveCursor(1,0);
}

void LCD_MainMenu(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+ : Open Door");
	LCD_displayStringRowColumn(1, 0, "- : Change Pass");

}

void LCD_Door_unlockPrint(void)
{
	LCD_clearScreen();
	LCD_displayString("Door is Unlocking");
}

void LCD_Door_lockPrint(void)
{
	LCD_clearScreen();
	LCD_displayString("Door is Locking");
	_delay_ms(100);
}

void HMI_timerCallBack(void)
{
	g_sec++;
}
