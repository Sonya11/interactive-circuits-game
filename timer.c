/*
 * timer.c
 *
 *  Created on: Oct 31, 2017
 *      Author: lemmo
 */


#include "timer.h"

extern volatile uint8_t timer_flag;

void configure_timer(void)
{
    // Timer to run the LCD display //
	TIMER_A0->CTL = 0x0034;	//Reset and halt timer for config
	TIMER_A0->CTL = 0x0100; //CLK = ACLK; div = 1
	TIMER_A0->CCTL[0] |= 0x0090;   //Enable timer interrupts and toggle mode
	TIMER_A0->CCR[0] = 31500;	//Reset value approx. equal to LFXT (aka ACLK) frequency (32kHz)
	TIMER_A0->CTL |= 0x0010;	//Start timer in up-mode
    NVIC_EnableIRQ(TA0_0_IRQn);

	// Timer to run the PWM wave //
	TIMER_A1->CTL = 0x0034; //Reset and halt timer for config
    TIMER_A1->CTL = 0x0200; //CLK = SMCLK, div = 1
    TIMER_A1->CCTL[0] |= 0x0080;  //Enable timer toggle mode
    TIMER_A1->CCTL[1] |= 0x00E0;  //Enable the next timer's interrupts in toggle/reset mode
    TIMER_A1->CCR[0] = 999;       //Reset value approx. equal to LFXT (aka ACLK) frequency (32kHz)
    TIMER_A1->CCR[1] = 250;       //Initialize the PWM to 100% duty cycle!
    TIMER_A1->CTL |= 0x0010;    //Start timer in up-mode
}
