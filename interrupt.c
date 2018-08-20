/*
 * interrupt.c
 *
 *  Created on: Oct 20, 2017
 *     
 */

#include "msp.h"
#include "interrupt.h"
#include "bluetooth.h"
#include "ADC.h"

extern uint8_t blue_screen;
extern uint8_t game_state;
extern uint8_t timer_flag;
extern uint8_t duty_flag;
extern uint8_t duty_val;

void CS_IRQHandler(void){
	CS->KEY = 0x695A; // Unlock CS module for register access
	CS->CLRIFG = 0x0343;	//Clear CSIFG
	CS->KEY = 0;	// lock CS module for register access
}

void PORT1_IRQHandler(void){
	if(P1IFG & 0x02){	//statement for P1.1
			P1IFG &= ~0x02;
			return;
	}
	if(P1IFG & 0x10){	//statement for P1.4
			P1IFG &= ~0x10;
			return;
	}
	return;
}

void TA0_0_IRQHandler(void)
 {
     TIMER_A0->CCTL[0] &= ~ 0x01; //clear flag
     timer_flag = 1;
 }

void EUSCIA3_IRQHandler(){
	if(EUSCI_A3->IFG & 0x01){	//statement for RXBUF receive
		EUSCI_A3->IFG &= ~0x01;	//clear RXBUF IFG
		uint8_t data[3];
        data [0] = EUSCI_A3->RXBUF;
		if(data[0] == '*'){	//command statement

			while(EUSCI_A3->STATW & 0x01);	//wait for next RXBUF receive
			data[0] = EUSCI_A3->RXBUF;
			switch(data){
				case '0' : blue_screen = 0;
				break;

				case '1' : blue_screen = 1;
				break;

				case '2' : blue_screen = 2;
				break;

				case '3' : blue_screen = 3;
				break;

				case 'A' : game_state = 1;
				break;

				case 'B' : game_state = 2;
				break;

				case 'C' : game_state = 3;
				break;

				case 'D' : game_state = 4;
				break;

				case 'P' :
				    while(EUSCI_A3->STATW & 0x01);   //wait for next RXBUF receive
				    data[0] = EUSCI_A3->RXBUF;
				    while(EUSCI_A3->STATW & 0x01);   //wait for next RXBUF receive
                    data[0] = EUSCI_A3->RXBUF;
                    while(EUSCI_A3->STATW & 0x01);   //wait for next RXBUF receive
                    data[0] = EUSCI_A3->RXBUF;
				    EUSCI_A3->IE &= ~0x01;
				    duty_flag = 1;
				break;
				}
			}
		else{	//non-command statement

		}
		return;
	}
}
