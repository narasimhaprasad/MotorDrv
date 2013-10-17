#include "motordrv.h"

/*
 * ADC processing function
 * Actual adc voltage = dig * 3.3/4095
 * All elements in Q13.19
 * Clocked in at 1.745us
 */
void adcproc(adc_t *A)
{
	int64_t acc3=0;
	acc3 = (int64_t)(A->buff[2]<<19)*0x1A6;	//Q13.19*Q13.19 = Q26.38
	A->ovoltage = (int32_t)(acc3>>19);	//Q26.38 >> 19 = Q13.19
	acc3 = (int64_t)(A->buff[1]<<19)*0x1A6;	//Q13.19*Q13.19 = Q26.38
	A->ocurrent = (int32_t)(acc3>>19);	//Q26.38 >> 19 = Q13.19
	acc3 = (int64_t)(A->buff[0]<<19)*0x1A6;	//Q13.19*Q13.19 = Q26.38
	acc3 -= (int64_t)0x1999980000;	//Q26.38 - 0.4<<38 = Q26.38
	A->ivoltage = (int32_t)(acc3>>19);	//Q26.38 >> 19 = Q13.19
}
