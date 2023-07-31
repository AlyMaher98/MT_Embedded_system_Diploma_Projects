/*
 * HMI_MCU.h
 *
 *  Created on: Jun 28, 2023
 *      Author: Aly Maher
 */

#ifndef HMI_MCU_H_
#define HMI_MCU_H_



/******************************************************************************
*                               INCLUDES			                          *
*******************************************************************************/

#include "../HAL/keypad.h"
#include "../HAL/lcd.h"
#include "../MCAL/gpio.h"
#include "../MCAL/uart.h"
#include "../LIBS/std_types.h"
#include "../LIBS/Utils.h"
#include "../MCAL/Timer.h"
#include <util/delay.h> /* For the delay functions */


#define F_CPU 8000000UL

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MESSAGE_DISPLAY_DELAY     2000

/* KEYPAD MACROS */
#define ENTER_KEY_PRESSED         13
#define KEYPAD_INPUT_DELAY        1000

/* PASSWORD MACROS */
#define PASSWORD_LENGTH           5
#define PASSWORD_MATCHED          TRUE
#define PASSWORD_UNMATCHED        FALSE

/* TIMING MACROS */
#define DOOR_UNLOCKING_PERIOD	            15
#define DOOR_LOCKING_PERIOD	                15
#define DOOR_LEFT_OPEN_PERIOD	            3
#define NUMBER_OF_WRONG_PASSWORD_ATTEMPTS 	3
#define KEYPAD_LOCKED_PERIOD			    60

/***** UART MACROS *****/
//Send & Receive Handlers
#define READY_TO_SEND               0x10
#define READY_TO_RECEIVE            0x20

//Change Password Handlers
#define CHANGE_PASSWORD_OPTION	    0x18
#define CHANGING_PASSWORD		    0x30

//Open Door Handlers
#define OPEN_DOOR_OPTION            0x19
#define UNLOCKING_DOOR			    0x31

//Wrong Password Handler
#define WRONG_PASSWORD			    0x25

/*********************************************************************
 *                          Global variables                         *
 ********************************************************************/

uint8 g_InputPassword[PASSWORD_LENGTH];   /* Global array to hold the values of the password entered by the user */
uint8 g_Password_Match_Status;            /* Global variable to hold the Matching status between the two password sent to Control ECU */
uint16 g_sec = 0;                         /* Global variable that is incremented inside Timer1 ISR every interrupt (1 Second) */
uint8 g_wrongPasswordCounter=0;           /* Global variable that is used as counter of number of wrong passwords entered */


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description: Function to get password as an input from the keypad and store it in a global array
 */
void HMI_getPassword(uint8 *password);

/*
 * Description: Function to send the entered password Byte by Byte to the Control ECU By UART
 */
void HMI_sendPasswordByUART(uint8 *password);
/*
 * Description: Function to Initialize System Password
 */
void HMI_SystemPasswordInit(uint8 *password);
/*
 * Description: Function to display write pass on the screen
 */
void LCD_WritePass(void) ;
/*
 * Description: Function to display Re-write pass on the screen
 */
void LCD_ReWritePass(void) ;
/*
 * Description: Function to display astrics (*) when we enter the password
 */
void LCD_Astricsprint(void);
/*
 * Description: Function to display write pass on the screen
 */
void LCD_WriteNewPass(void) ;
/*
 * Description: Function to display the application main options
 */
void LCD_MainMenu(void) ;
/*
 * Description: A function that displays on LCD that door is opening for a certain period of time
 */
void LCD_Door_unlockPrint(void);
/*
 * Description: A function that displays on LCD that door is closing for a certain period of time
 */
void LCD_Door_lockPrint(void);
/*
 * Description: Timer Call Back Function the is related to HMI Module
 */
void HMI_timerCallBack(void);

/*
 * Description: A function that displays on LCD that door is opening or closing for a certain period of time
 */
void HMI_OpenDoor(void);



#endif /* HMI_MCU_APP_HMI_MCU_H_ */