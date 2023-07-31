/*
 * Control_MCU.h
 *
 *  Created on: Jul 2, 2023
 *      Author: Aly Maher
 */

#ifndef CONTROL_MCU_H_
#define CONTROL_MCU_H_

/******************************************************************************
*                               INCLUDES			                          *
*******************************************************************************/


#include "../MCAL/gpio.h"
#include "../MCAL/uart.h"
#include "../LIBS/std_types.h"
#include "../LIBS/Utils.h"
#include "../MCAL/Timer.h"
#include "../HAL/external_eeprom.h"
#include "../HAL/dc_motor.h"
#include "../HAL/buzzer.h"
#include "../MCAL/twi.h"
#include <util/delay.h> /* For the delay functions */


#define F_CPU 8000000UL

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MESSAGE_DISPLAY_DELAY     2000

/* KEYPAD MACROS */
#define ENTER_KEY_PRESSED         13
#define KEYPAD_INPUT_DELAY        2200

/* PASSWORD MACROS */
#define PASSWORD_LENGTH           5
#define PASSWORD_MATCHED          TRUE
#define PASSWORD_UNMATCHED        FALSE

/* TWI & EEPROM MACROS */
#define TWI_CONTROL_ECU_ADDRESS				0x01
#define EEPROM_STORE_ADDREESS				0x00


/* TIMING MACROS */
#define DOOR_UNLOCKING_PERIOD	            15
#define DOOR_LOCKING_PERIOD	                15
#define DOOR_LEFT_OPEN_PERIOD	            3
#define NUMBER_OF_WRONG_PASSWORD_ATTEMPTS 	3
#define ALARM_ON_DELAY						60

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

uint8 g_receivedPassword[PASSWORD_LENGTH];   /* Global array to hold the values of the received password from HMI ECU */
uint8 g_storedPassword[PASSWORD_LENGTH];     /* Global array to store the values of the password In EEPROM if password is correct */
uint8 g_wrongPasswordCounter=0;              /* Global variable that is used as counter of number of wrong passwords entered */
uint16 g_sec = 0;                            /* Global variable that is incremented inside Timer1 ISR every interrupt (1 Second) */

/*******************************************************************************
 *                           Functions Prototypes                              *
 *******************************************************************************/

/*
 * Description: a function to compare the stored pass with the received one
 */
uint8 CTRL_comparePasswords(const uint8 *a_password1,const uint8 *a_password2);

/*
 * Description: a function to initialize the password in first-run OR to change the password
 */
void CTRL_SystemPasswordInit(uint8 *pass);

/* Description:
 *    A function that 1) Rotates on the DC motor for 15 seconds clockwise,
 *                    2) Stops it for 3 seconds,
 *                    3) Rotates it anti-clockwise for 15 seconds.
 */
void CTRL_OpenDoor(void);

/*
 * Description: the call-back function called by the timer every 1 second
 */
void CTRL_timerCallBack(void);

/*
 * Description: A function to receive the password via UART by looping on receiveByte function
 */
void CTRL_receivePasswordByUART(uint8 * pass);

/*
 * Description: A function to retrieve the stored password from EEPROM
 */
void CTRL_updateStoredPassword(void);

/*
 * Description: A function to store the password in EEPROM
 */
void CTRL_storePassword(void);

#endif /* APP_CONTROL_MCU_H_ */
