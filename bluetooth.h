/*
 * bluetooth.h
 *
 *  Created on: Nov 26, 2017
 *      Author: lemmo
 *
 *
 */
#include "msp.h"
#ifndef BLUTOOTH_H_
#define BLUTOOTH_H_


void configure_blue(void);
void blue_master(void);
void blue_resist(void);
void blue_avg(void);
void blue_int(uint8_t cmd, uint16_t value);
void blue_float(uint8_t cmd, float value);
void UART_send(uint8_t * data, uint32_t length);
void UART_send_byte(uint8_t data);
void UART_int(uint16_t integer);
void UART_float(float number);
void duty_set(void);




#endif /* BLUTOOTH_H_ */
