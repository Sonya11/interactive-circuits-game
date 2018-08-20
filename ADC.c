#include "msp.h"
/*
 * ADC.c
 *
 *  Created on: Nov 28, 2017
 *     
 */

#include "ADC.h"

extern volatile float volt_val;
void configure_ADC(void)
{
    // Configure the Reference Module
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY); // Wait if reference generator is busy
    REF_A->CTL0 |= REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;   // Set up a 1.2 V reference voltage
    ADC14->CTL0 &= ~(ADC14_CTL0_ENC);   // Turn off our ADC so that we can edit its values

    // Configure ports P4.7 to ADC inputs
    // Set P4.7 to ADC tertiary mode
    P4->SEL0 |= BIT7;
    P4->SEL1 |= BIT7;

    // Configure our ADC
    // Joystick Sensor Sample time > 30us, ADC ON!, Select SMCLK as our input clock!
    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_2 | ADC14_CTL0_MSC; // Configure ADC for Single Channel Multiple conversion
    ADC14->CTL0 |= ADC14_CTL0_SHT0_5 | ADC14_CTL0_SHP | ADC14_CTL0_ON | ADC14_CTL0_SSEL_4;
    ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;                    // 14 bit resolution
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_6;                    // Map to Memory 0, 3.3 VREF

    while(REF_A->CTL0 & REF_A_CTL0_GENRDY); // Wait for reference generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC;          // ADC14 enable conversion
    ADC14->CTL0 |= ADC14_CTL0_SC;           // Start our ADC conversion
}
