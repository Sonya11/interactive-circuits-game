/*
 * bluetooth.c
 *
 *  Created on: Nov 26, 2017
 *      Author: lemmo
 */
#include "msp.h"
#include "bluetooth.h"

extern volatile float volt_val;
extern volatile uint16_t pot_resist;
extern volatile float pot_cur;
extern uint8_t duty_val;

void configure_blue(void){
	//UART-P9.6:RX; P9.7:TX (P9.6/P9.7 Primary)
	P9->SEL0 |= 0xC0;
	P9->SEL1 &= 0xC0;

	// Configure UART (115200 Baud)
	EUSCI_A3->CTLW0 |= 0x01; // Put eUSCI in reset

	EUSCI_A3->CTLW0 = 0x0081; // Select Frame parameters and clock source
	EUSCI_A3->BRW = 0x000D; // Set Baud Rate
	EUSCI_A3->MCTLW = 0x2501; // Set first stage modulator bits (if necessary)

	EUSCI_A3->CTLW0 &= ~0x01; // Initialize eUSCI
	EUSCI_A3->IE = 0x01; // Enable RXBUF interrupts
}

void blue_master(){

}

void blue_resist(){
	blue_int('N', ADC14->MEM[0]);
	blue_float('V', volt_val);
	blue_float('C', pot_cur*1000);
	blue_int('R', pot_resist);
}

void blue_avg(){

}

void blue_float(uint8_t cmd, float value){
    UART_send_byte('*');
    UART_send_byte(cmd);
    UART_float(value);
    UART_send_byte('*');
}
void blue_int(uint8_t cmd, uint16_t value){
	UART_send_byte('*');
	UART_send_byte(cmd);
	UART_int(value);
	UART_send_byte('*');
}

void UART_send(uint8_t * data, uint32_t length){ //data is uint8_t so that pointer increments by 8 bits
    uint8_t i;
    for(i = 0; i < length; i++){
        UART_send_byte(*(data+i));
    }
}

void UART_send_byte(uint8_t data){
    while(UCA3STATW & 0x01); //while the UART transmit or receive is busy
    EUSCI_A3->TXBUF = data;
}

void UART_int(uint16_t integer){
	UART_send_byte(((integer % 100000)/10000)+48);
	UART_send_byte(((integer % 10000)/1000)+48);
	UART_send_byte(((integer % 1000)/100)+48);
    UART_send_byte(((integer % 100)/10)+48); //converts to ascii value based on ascii integer offset
    UART_send_byte(((integer % 10)/1)+48);
}

void UART_float(float number){
    //seperates the numbers before and after the decimal point
    uint8_t integer = (uint8_t)number/1;
    uint16_t decimal = (number - integer)*1000;

    UART_send_byte(integer+48);
    UART_send_byte(0x2E); //decimal point
    UART_send_byte(((decimal % 1000)/100)+48);
    UART_send_byte(((decimal % 100)/10)+48);
    UART_send_byte(((decimal % 10)/1)+48);
}

void duty_set(){
    uint8_t duty_val = 0;
    while(EUSCI_A3->RXBUF != '*'){
        while(!(EUSCI_A3->IFG & 0x01));  //wait for next RXBUF recieve
        EUSCI_A3->IFG &= ~0x01;
        duty_val = (duty_val*10+(EUSCI_A3->RXBUF-48));
    }
    EUSCI_A3->IE |= 0x01;
    TIMER_A1->CCR[1] = duty_val*10;
    blue_int('X', duty_val);
}
