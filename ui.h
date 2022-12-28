/* 
 * File:        ui.h
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

#ifndef _UI__H_
#define _UI__H_

#include <xc.h>
#include "type.h"
#include "uart.h"
#include "pos.h"

//=============USER FUNCTIONS=============

void UIPageMain() {
    UartCurShow(0);
    UartSendString("\f");
    UartSendUTF8(0xe2, 0x94, 0x8c);
    for (uint8_t i = 0; i < 64; i++) {
        UartSendUTF8(0xe2, 0x94, 0x80);
    }
    UartSendUTF8(0xe2, 0x94, 0x90);
    for (uint8_t i = 0; i < 20; i++) {
        UartSendString("\n\r");
        UartSendUTF8(0xe2, 0x94, 0x82);
        if (i == 5) {
            for (uint8_t j = 0; j < 25; j++) UartSendChar(' ');
            UartSendString("\xe4\xbf\x84 \xe7\xbd\x97 \xe6\x96\xaf \xe6\x96\xb9 \xe5\x9d\x97");
            for (uint8_t j = 0; j < 25; j++) UartSendChar(' ');
        } else if (i == 7) {
            for (uint8_t j = 0; j < 25; j++) UartSendChar(' ');
            UartSendString("\xe6\xa2\x85\xe5\x8a\xaa\xe6\x96\xaf\xe7\x94\xb5\xe4\xbf\xa1\xe4\xb8\x89\xe7\x8f\xad");
            for (uint8_t j = 0; j < 25; j++) UartSendChar(' ');
        } else if (i == 9) {
            for (uint8_t j = 0; j < 25; j++) UartSendChar(' ');
            UartSendString("\xe5\xbc\xa0\xe9\x92\x8a\xe6\xb4\x8b  \xe9\x99\x88\xe6\x81\xba\xe4\xb8\xb0");
            for (uint8_t j = 0; j < 25; j++) UartSendChar(' ');
        } else if (i == 15) {
            for (uint8_t j = 0; j < 25; j++) UartSendChar(' ');
            UartSendString("\xef\xbc\x88\xe6\x8c\x89\xe4\xb8\x8b\xe5\x9b\x9e\xe8\xbd\xa6\xe9\x94\xae\xef\xbc\x89");
            for (uint8_t j = 0; j < 25; j++) UartSendChar(' ');
        } else {
            for (uint8_t j = 0; j < 64; j++) UartSendChar(' ');
        }
        UartSendUTF8(0xe2, 0x94, 0x82);
    }
    UartSendString("\n\r");
    UartSendUTF8(0xe2, 0x94, 0x94);
    for (uint8_t i = 0; i < 64; i++) {
        UartSendUTF8(0xe2, 0x94, 0x80);
    }
    UartSendUTF8(0xe2, 0x94, 0x98);
}

void UIClearGame() {
    UartCurSet(24, 2);
    for (uint8_t i = 0; i < 20; i++) {
        UartSendString("                    ");
        if (i != 19) UartCurMove(-20, 1);
    }
}

void UIPageMainToGame() {
    UIClearGame();
    UartCurSet(44, 1);
    UartSendString("\xe2\x94\xac");
    for (uint8_t i = 2; i < 22; i++) {
        UartCurMove(-1, 1);
        UartSendUTF8(0xe2, 0x94, 0x82);
    }
    UartCurMove(-1, 1);
    UartSendUTF8(0xe2, 0x94, 0xB4);
    UartCurSet(23, 1);
    UartSendUTF8(0xE2, 0x94, 0xac);
    for (uint8_t i = 2; i < 22; i++) {
        UartCurMove(-1, 1);
        UartSendUTF8(0xe2, 0x94, 0x82);
    }
    UartCurMove(-1, 1);
    UartSendUTF8(0xe2, 0x94, 0xB4);
    UartCurSet(50, 4);
    UartSendString("\xe2\x94\x8c");
    for (uint8_t i = 0; i < 10; i++) UartSendString("\xe2\x94\x80");
    UartSendString("\xe2\x94\x90");
    for (uint8_t i = 0; i < 4; i++) {
        UartCurMove(-12, 1);
        UartSendString("\xe2\x94\x82          \xe2\x94\x82");
    }
    UartCurMove(-12, 1);
    UartSendString("\xe2\x94\x94");
    for (uint8_t i = 0; i < 10; i++) UartSendString("\xe2\x94\x80");
    UartSendString("\xe2\x94\x98");
    UartCurSet(52, 3);
    UartSendString("\xe4\xb8\x8b\xe6\xac\xa1\xE5\x87\xBA\xE7\x8E\xB0");
}

//End ADC 8 Bit Interfacing Functions

#endif