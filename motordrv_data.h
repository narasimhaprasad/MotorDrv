/*
 * motorinv.h
 *
 *  Created on: Sep 23, 2013
 *      Author: elenpp
 */

#ifndef MOTORINV_DATA_H_
#define MOTORINV_DATA_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint32_t cycle;
	uint32_t period;
	uint32_t deadband;
}inv_t;

typedef struct
{
	uint32_t cycle;
	uint32_t period;
	uint32_t deadband;
}conv_t;

typedef struct
{
	int32_t ovoltage;
	int32_t ocurrent;
	int32_t ivoltage;
	uint32_t buff[3];
}adc_t;

typedef struct
{
	int32_t a0;
	int32_t a1;
	int32_t state[2];
	int32_t kp;
	int32_t ki;
	int32_t ref;
	uint8_t status;
}pid_t;

typedef enum{
	state_0,
	state_1,
	state_2
}state_t;

typedef enum{
	noact,
	act_0,
	act_1,
	act_2,
	act_3,
	act_4
}act_t;

typedef enum{
	noevt,
	sw1,
	sw2
}evt_t;

typedef struct{
	state_t nextState;
	act_t actionToDo;
}statemat_t;

extern inv_t inv;
extern conv_t conv;
extern adc_t adc;
extern pid_t vpid;
extern pid_t ipid;
extern statemat_t statematrix[3][3];
extern state_t currentstate;

#endif /* MOTORINV_H_ */
