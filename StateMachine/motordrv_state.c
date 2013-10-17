#include "motordrv_data.h"
#include "motordrv.h"

act_t stateeval(evt_t e)
{
	statemat_t statetrans = statematrix[currentstate][e];

	currentstate = statetrans.nextState;

	return statetrans.actionToDo;
}

void stateaction(act_t a)
{
	switch (a)
	{
	uint8_t j;
	case act_0:
		PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT|PWM_OUT_1_BIT, false);
		PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, false);
//		IntDisable(INT_PWM1_1);
		IntDisable(INT_TIMER1A);
		break;

	case act_1:
		conv.cycle = 500*conv.period/1000;
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, conv.cycle);
		PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT|PWM_OUT_1_BIT, true);
		PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);

//		IntDisable(INT_PWM1_1);
//		IntDisable(INT_TIMER1A);
		break;

	case act_2:
		for(j=0; j<2; j++)
			{
				vpid.state[j] = 0;
			}
		for(j=0; j<2; j++)
			{
				ipid.state[j] = 0;
			}
		vpid.state[1] = adc.ocurrent;
		ipid.state[1] = conv.cycle;
//		IntEnable(INT_PWM1_1);
		IntEnable(INT_TIMER1A);
		break;
	}
}
