/* 
 * File:        uart.h
 * Modified by: rsooio
 *
 * Modified on 8 Dec 2022, 1:01
 */

/***********USER FUNCTIONS***********

1.	void UartInit()
                - Must be called to initialise UART.
                - Note what SFRs are effected and be sure not to overwrite these in yourt program initialisation.

2.	void UartClear()
                - Call this to Clear Serial display

4.	void UartSendChar(char)
                - Write a character to the Serial e.g.      UartSendChar('A');

5.	void UartSendString(char*)
                - Write a string to the Serial e.g.         UartSendString("Hello");

6.	uint8_t UartRecvChar()
                - Receive a character from the Serial e.g.  uint8_t ch = UartRecvChar();

7.	uint8_t *UartRecvString()
                - Not implemented at present.
                - Receive a string from the Serial e.g.     uint8_t *ch = UartRecvString();

 */

#ifndef _UART__H_
#define _UART__H_

#include <xc.h>
#include "type.h"

//=============USER FUNCTIONS=============

void UartInit(void) // setup stuff
{
    TRISC |= 0xC0; // RC6 and RC7 must be set to inputs for USART.
    TXSTA = 0x24; // Set TXEN bit to enable transmit.
    // Set BRGH bit for Baud Rate table selection.
    TRISB = 0x03;
    PORTC = 0x00;
    RCSTA = 0x90; // Set CREN bit for continuous read.
    // Set SPEN bit to enable serial port.
//    SPBRG = 0x19; // Set Baud Rate to 9600
//    SPBRG = 0x0c; // Set Baud Rate to 19231
    SPBRG = 0; // Set Baud Rate to 250000
}

void UartSendChar(uint8_t ch) {
    while (!TXIF); // set when transmit register (TXREG) is empty
    TXREG = ch; // 0x41 is the ASCII value for the letter ?A?
    // or you can use TXREG = ?A?
    // Once TXREG is loaded the byte is automatically
    // transmitted by the USART peripheral
    while (!TRMT);
}

void UartSendUTF8(uint8_t a, uint8_t b, uint8_t c) {
    UartSendChar(a);
    UartSendChar(b);
    UartSendChar(c);
}

void UartSendString(const uint8_t *str) {
    while (*str) {
        UartSendChar(*str);
        str++;
    }
}

void UartSendEsc(uint8_t *str) {
    UartSendChar(0x1b);
    UartSendString(str);
}

uint8_t _UartRecvChar(void) {
    while (!RCIF);
    return RCREG;
}

uint8_t UartRecvChar(void) {
    CREN = 1;
    uint8_t ch = _UartRecvChar();
    CREN = 0;
    return ch;
}

void UartCurSet(uint8_t x, uint8_t y) {
    UartSendString("\033[");
    if (y / 10) UartSendChar(y / 10 + '0');
    UartSendChar(y % 10 + '0');
    UartSendChar(';');
    if (x / 10) UartSendChar(x / 10 + '0');
    UartSendChar(x % 10 + '0');
    UartSendChar('H');
}

void UartCurMove(int8_t x, int8_t y) {
    UartSendString("\033[");
    if (y > 0) {
        if (y / 10) UartSendChar(y / 10 + '0');
        UartSendChar(y % 10 + '0');
        UartSendChar('B');
    } else {
        if (-y / 10) UartSendChar(-y / 10 + '0');
        UartSendChar(-y % 10 + '0');
        UartSendChar('A');
    }
    UartSendString("\033[");
    if (x > 0) {
        if (x / 10) UartSendChar(x / 10 + '0');
        UartSendChar(x % 10 + '0');
        UartSendChar('C');
    } else {
        if (-x / 10) UartSendChar(-x / 10 + '0');
        UartSendChar(-x % 10 + '0');
        UartSendChar('D');
    }
}

void UartCurShow(int8_t show) {
//    UartSendString("\x1b[?25");
//    UartSendChar(show ? 'h' : 'l');
}

// End Uart Interfacing Functions

#endif




