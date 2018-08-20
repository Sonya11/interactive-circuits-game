/*
 * lcd.h
 *
 *  Created on: Nov 25, 2017
 *  
 */

#ifndef LCD_H_
#define LCD_H_

#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h> ///home/sonya/ti/simplelink_msp432p4_sdk_1_50_00_12/source   <ti/devices/msp432p4xx/driverlib/driverlib.h> <driverlib/MSP432P4xx/driverlib.h>
#include <ti/grlib/grlib.h>
#include <LcdDriver/Crystalfontz128x128_ST7735.h>

void lcd_setup(Graphics_Context * context_address);

#endif /* LCD_H_ */
