/*
 * motorinv.h
 *
 *  Created on: Sep 23, 2013
 *      Author: elenpp
 */

#ifndef MOTORINV_DATA_H_
#define MOTORINV_DATA_H_

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

extern inv_t inv;
extern conv_t conv;
extern adc_t adc;
extern pid_t vpid;
extern pid_t ipid;

#endif /* MOTORINV_H_ */
