#include "motordrv.h"

void hardware_init(void)
{
	//Set PWM clock at the same frequency as system clock
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	/*
	 * Inverter system setup
	 * period : Switching Frequency : 20Khz
	 * cycle : Duty Cycle SPWM
	 * deadband : Deadband
	 */
	inv.period = SysCtlClockGet()/20000;
	inv.deadband = 25*inv.period/100;
	inv.cycle = inv.period*500/1000;

	/*
	 * Boost converter system setup
	 * period: Switching frequency: 20kHz
	 * cycle: Duty cycle
	 * deadband: Deadband
	 */
	conv.period = inv.period;
	conv.deadband = inv.deadband;
	conv.cycle = conv.period*500/1000;

	/*
	 * Setup SPWM on PWM0 GEN0 and GEN1
	 * PB6 : PWM0 GEN0
	 * PB7: PWM0 GEN1
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	SysCtlPeripheralReset(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOB);

	GPIOPinConfigure(GPIO_PB6_M0PWM0);
	GPIOPinConfigure(GPIO_PB7_M0PWM1);

	GPIOPinTypePWM(GPIO_PORTB_BASE, (GPIO_PIN_6 | GPIO_PIN_7));

	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, (PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_GEN_SYNC_LOCAL | PWM_GEN_MODE_FAULT_UNLATCHED | PWM_GEN_MODE_DB_NO_SYNC));
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, inv.period);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, inv.cycle);
	PWMDeadBandEnable(PWM0_BASE, PWM_GEN_0, 13, 0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT|PWM_OUT_1_BIT, true);

	PWMSyncUpdate(PWM0_BASE, PWM_OUT_0_BIT|PWM_OUT_1_BIT);

	/*
	 * Setup boost converter pwm on PWM1
	 * PA6: PWM1 GEN1
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	SysCtlPeripheralReset(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA6_M1PWM2);	//Map PWM1, P1 OP2 to PA6
	GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6);	//Configure PA6 as PWM

	PWMGenConfigure(PWM1_BASE, PWM_GEN_1, (PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_GEN_NO_SYNC| PWM_GEN_MODE_DB_NO_SYNC));	//Configure PWM1, G1 as Down counter with no sync of updates
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, conv.period);	//Set Period of PWM1, G1
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, conv.cycle);	//Set phase shift

	PWMIntEnable(PWM1_BASE, PWM_INT_GEN_1);
	PWMGenIntTrigEnable(PWM1_BASE, PWM_GEN_1, PWM_TR_CNT_LOAD|PWM_INT_CNT_LOAD);
	IntPrioritySet(INT_PWM1_1, 0x02);
	PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);

	PWMGenEnable(PWM1_BASE, PWM_GEN_1);

	/*
	 * Setup ISR for updating SPWM duty cycle
	 */
	uint32_t ui32TimIntSine = SysCtlClockGet()/200000;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

	SysCtlPeripheralReset(SYSCTL_PERIPH_TIMER2);

	TimerConfigure(TIMER2_BASE, TIMER_CFG_A_PERIODIC);
	TimerLoadSet(TIMER2_BASE, TIMER_A, ui32TimIntSine - 1);

	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_TIMER2A, 0x00);
	TimerEnable(TIMER2_BASE, TIMER_A);

	uint32_t ui32TimIntIcontrol = SysCtlClockGet()/40000;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	SysCtlPeripheralReset(SYSCTL_PERIPH_TIMER1);

	TimerConfigure(TIMER1_BASE, TIMER_CFG_A_PERIODIC);
	TimerLoadSet(TIMER1_BASE, TIMER_A, ui32TimIntIcontrol - 1);

	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_TIMER1A, 0x01);
	TimerEnable(TIMER1_BASE, TIMER_A);

	/*
	 * Setup PF1 as debug pin
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

	/*
	 * Setup ADC
	 * Configuration currently in discussion
	 * Interrupt at end might not be necessary
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOE);

	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	SysCtlPeripheralReset(SYSCTL_PERIPH_ADC0);

	ADCHardwareOversampleConfigure(ADC0_BASE, 2);
	ADCSequenceDisable(ADC0_BASE, 2);

	ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PWM1, 0x00);
	HWREG(0x4003801C) |= 0x1000;	//Sec 13.4.2 Pt 3

	ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH1);

	ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH2);

	ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH3|ADC_CTL_IE|ADC_CTL_END);

	ADCSequenceEnable(ADC0_BASE, 2);
}

void software_init(void)
{
	/*
	 * PID connfiguration
	 * All elements have to be in 13.19 format
	 * A0 = Kp + Ki
	 * A1 = -Kp
	 */
	vpid.kp = 0x8000;
	vpid.ki = 0x4000;

	ipid.kp = 0x8000;
	ipid.ki = 0x4000;

	vpid.a0 = vpid.kp + vpid.ki ;
	vpid.a1 = -vpid.kp;

	ipid.a0 = ipid.kp + ipid.ki;
	ipid.a1 = -ipid.kp;
}
