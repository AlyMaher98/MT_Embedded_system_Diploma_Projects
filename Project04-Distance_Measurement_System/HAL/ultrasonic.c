 /******************************************************************************
 *
 * Module: Ultrasonic
 *
 * File Name: ultrasonic.c
 *
 * Description: application to measure the distance using ultrasonic sensor
 *
 * Author: Aly Maher
 *
 *******************************************************************************/
#include "ultrasonic.h"

uint16 timecounter;
uint16 distance;

void Ultrasonic_init(void)
/*
 * Description :
 * 1-Initialize the ICU driver as required.
 * 2-Setup the ICU call back function.
 * 3-Setup the direction for the trigger pin as output pin through the GPIO driver.
 * 		Inputs: None
 * 		Return: None
*/
{
	Icu_ConfigType ICU_config = {F_CPU_8,RISING};
	Icu_init(&ICU_config);
	Icu_setCallBack(&Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(PORTB_ID,PIN5_ID, PIN_OUTPUT);

}
void Ultrasonic_Trigger(void)
/*
 * Description :
 * 	1-Send the Trigger pulse to the ultrasonic.
 * 		Inputs: None
 * 		Return: None
*/
{
	GPIO_writePin(PORTB_ID,PIN5_ID, LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(PORTB_ID,PIN5_ID, LOGIC_LOW);
}
uint16 Ultrasonic_readDistance(void)
/*
 * Description:
 * Send the trigger pulse by using Ultrasonic_Trigger function.
 * Start the measurements by the ICU from this moment.
 * Inputs: None
 * Return: The measured distance in Centimeter.
*/
{

	Ultrasonic_Trigger();
	distance = (timecounter*34600)/(1000000*2);
	Icu_setEdgeDetectionType(RISING);
	return distance;
}

void Ultrasonic_edgeProcessing(void)
/*
 * Description :
 * This is the call back function called by the ICU driver.
 * This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 * Inputs: None
 * Return: None
*/
{
	Icu_clearTimerValue();
	Icu_setEdgeDetectionType(FALLING);
	while(!(BIT_IS_SET(TIFR,ICF1)));
	timecounter=Icu_getInputCaptureValue();
}
