 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.h
 *
 * Description: header file for the ATmega16 ADC driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "../LIBS/Types.h"
#include "../LIBS/Utils.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   2.56

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
enum ADC_ReferenceVolatge {
	AREF, AVCC, RESERVED, INTERNAL256
};
enum ADC_Prescaler {
	FCPU, FCPU2, FCPU4, FCPU8, FCPU16, FCPU32, FCPU64, FCPU128
};
typedef struct {
	enum ADC_ReferenceVolatge ref_volt;
	enum ADC_Prescaler prescaler;
} ADC_ConfigType;

void ADC_init(const ADC_ConfigType *Config_Ptr);

/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */
short ADC_readChannel(uint8_t channel_num);

#endif /* ADC_H_ */
