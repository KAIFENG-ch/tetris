/* 
 * File:        interrupt.h
 * Modified by: rsooio
 *
 * Modified on 19 Oct 2022, 22:44
 */

/***********USER FUNCTIONS***********

1.	AdcInit()
                - Must be called to initialise ADC.
                - Note what SFRs are effected and be sure not to overwrite these in yourt program initialisation.

2.	AdcVolt()
                - Call this to Clear LCD display

 */

#ifndef _ADC__H_
#define _ADC__H_

#include <xc.h>
#include "type.h"

#ifndef _XTAL_FREQ
// Unless already defined assume 4MHz system frequency
// This definition is required to calibrate __delay_us() and __delay_ms()
#define _XTAL_FREQ 4000000
#endif 


//=============USER FUNCTIONS=============

void AdcInit() {
    ADCON0 = 0x11;
    ADCON1 = 0x02;
    TRISA = 0x04;
}

void AdcSetChan(uint8_t chan) {
    ADCON0 &= 0xc7;
    ADCON0 |= chan << 3;
}

void AdcGo() {
    ADCON0 |= 0x04;
}

void _AdcWait() {
    while (!ADIE & !ADIF);
}

uint8_t AdcVoltH() {
    _AdcWait();
    return ADRESH;
}

uint8_t AdcVoltL() {
    _AdcWait();
    return ADRESL;
}

uint16_t AdcVoltA() {
    _AdcWait();
    return (ADRESH << 8) | ADRESL;
}

//End ADC 8 Bit Interfacing Functions

#endif