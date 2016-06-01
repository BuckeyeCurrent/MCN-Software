/*
 * clock.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */
#include "../MCN2 Headers/all.h"


clock_struct Clock_Ticks = CLOCK_TICKS_CLEAR;

void ClockSetup()
{
	SystemClockSetup();
	InitializeCpuTimer2(CLOCK_PERIOD);
	StartCpuTimer2();
}

// Connected to INT13 of CPU (use MINT13 mask):
__interrupt void INT13_ISR(void)     // INT13 or CPU-Timer1
{
	 //***********************************WARNING!!********************************************\\
	//BE CAREFUL YOU NEED TO ALLOW NESTING FOR ANY INTERRUPT THAT MIGHT HAPPEN IN THIS INTERRUPT\\

	EINT;		//enable all interrupts

	ClockHeartbeat(user_ops.UserFlags.all);

	RestartCPUTimer1();
	DINT;
}

// Connected to INT14 of CPU (use MINT14 mask):
// ISR can be used by the user.
__interrupt void INT14_ISR(void)     // INT14 or CPU-Timer2
{
	//***********************************WARNING!!********************************************\\
	//BE CAREFUL YOU NEED TO ALLOW NESTING FOR ANY INTERRUPT THAT MIGHT HAPPEN IN THIS INTERRUPT\\

	 EINT;		//enable all interrupts

	//todo USER: Define Clock ISR

	Clock_Ticks.group1_ticks++;
	//Clock_Ticks.group2_ticks++;
	//Clock_Ticks.group3_ticks++;

	if (Clock_Ticks.group1_ticks >= FIVE_HZ_TICKS)
	{
		//send data or fill data
		SendCAN(MOTOR_COOLANT_TEMPS_BOX);
		SendCAN(CONTROLLER_COOLANT_TEMPS_BOX);
		SendCAN(CONTACTOR_BOX_TEMP_BOX);
		SendCAN(COOLANT_FLOWS_BOX);
		Clock_Ticks.group1_ticks = 0;
	}

	RestartCpuTimer2();
	DINT;

}

