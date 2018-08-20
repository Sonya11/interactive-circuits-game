//*****************************************************************************
//
// MSP432 - Scooter Design
// Garret Lemmon
// ECEN 2440
//
//****************************************************************************

#include "msp.h"
#include "default.h"
#include "port.h"
#include "clock.h"
#include "timer.h"
#include "bluetooth.h"
#include "ADC.h"
#include "game.h"
#include "lcd.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>

//Stucture initialization
Graphics_Context lcd_context;

//Global variables
uint8_t blue_screen = 0;
uint8_t game_state = 0;
uint8_t timer_flag = 0;
uint8_t duty_flag = 0;
uint8_t duty_val = 0;


void main(void){
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    __enable_interrupt(); // This enables interrupts globally for the CPU
    SYSCTL->NMI_CTLSTAT &= ~0x01;	//Disable CS as a source of NMI
    NVIC->ISER[0] |= 1 << 1;	//Enable CS interrupt
//	NVIC->ISER[1] |= 1 << 3;	//Enable Port1 interrupt
	NVIC->ISER[0] |= 1 << 8;	//Enable Timer_A0 CCTL0 interrupt
	NVIC->ISER[0] |= 1 << 19;	//Enable EUSCI_A3 interrupt

    default_config();
    configure_pcm();
    configure_clock();
    configure_port();
    configure_blue();
    configure_timer();
    configure_ADC();
    lcd_setup(&lcd_context);
    UART_send("Configure Complete\n", 19);

    wait_to_begin();
    UART_send("Begin Game\n", 11);

    while(1){
    	//game
    	play_game();

    	//bluetooth case
    	switch(blue_screen){
    		case(0) : break; //Nothing

    		case(1) : UART_send("Transmitting to Master Terminal\n", 32);
    		          blue_master();
    		          break;

    		case(2) : UART_send("Transmitting to Resistor Terminal\n", 34);
    		          blue_resist(); //wait to begin
    		          break;

    		case(3) : UART_send("Transmitting to Averaging Terminal\n", 35);
    		          blue_avg();
    		          break;
    	}
    	blue_screen = 0;

    }
}
