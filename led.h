/* 
 * File:        led.h
 * Modified by: rsooio
 *
 * Modified on 8 Dec 2022, 1:24
 */

/***********USER FUNCTIONS***********

1.	void LedInit()
				- Must be called to initialise LED.
				- Note what SFRs are effected and be sure not to overwrite these in yourt program initialisation.

2.	void LedClear()
				- Call this to Clear Led display

4.	void LedSetGrn(bool)
				- Set Green Led State e.g.              SetGreenLed(true);

5.	void LedSetRed(bool)
				- Set Red Led State e.g.                SetRedLed(false);

6.	void LedSetBar(uint8_t)
                - Set Led Bar State e.g.                SetLedBar(0xaa);

7.	void LedSetBar(uint8_t, bool)
				- Set some pin of Led Bar State e.g.    SetLedBar(0xaa, false);

*/

#ifndef _LED__H_
#define _LED__H_

#include <xc.h>
#include "type.h"

#ifndef _XTAL_FREQ
 // Unless already defined assume 4MHz system frequency
 // This definition is required to calibrate __delay_us() and __delay_ms()
 #define _XTAL_FREQ 4000000
#endif 

//=============USER FUNCTIONS=============

void LedInit(void)
{
    TRISC &= 0xfc;           // RC0 to RC1 must be set to outputs for LED Green and Red.
    TRISD = 0x00;            // RD0 to RD7 must be set to outputs for LED Bar.
    PORTC &= 0xfc;           // Set RC0 to RC1 0 to turn off LED Green and Red.
    PORTD = 0xff;            // Set RD0 to RD7 0 to turn off LED Bar.
}

void LedClear(void)
{
    PORTC &= 0xfc;           // Set RC0 to RC1 0 to turn off LED Green and Red.
    PORTD = 0xff;            // Set RD0 to RD7 0 to turn off LED Bar.
}

void LedSetGrn(uint8_t state)
{
    RC0 = state;
}

void LedSetRed(uint8_t state)
{
    RC1 = state;
}

uint8_t LedGetGrn(void)
{
    return RC0;
}

uint8_t LedGetRed(void)
{
    return RC1;
}

void LedSetBar(uint8_t state)
{
    PORTD = ~state;
}

void LedSetBarPin(uint8_t pin, uint8_t state)
{
    if (state)
        PORTD &= ~pin;
    else
        PORTD |= pin;
}

// End Uart Interfacing Functions

#endif




