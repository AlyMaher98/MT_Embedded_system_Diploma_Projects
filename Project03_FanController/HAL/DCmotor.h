/*
 * DCmotor.h
 *
 *  Created on: Jun 25, 2023
 *      Author: Aly Maher
 */

#include<stdio.h>

#include "..\LIBS\Types.h"

enum DcMotor_State {CW , ACW , stop};

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

void DcMotor_Init(void);

void DcMotor_Rotate(enum DcMotor_State state,uint8_t speed);

#endif /* DCMOTOR_H_ */
