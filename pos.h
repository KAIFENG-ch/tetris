/* 
 * File:        pos.h
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

#ifndef _POS__H_
#define _POS__H_

#include <xc.h>
#include "type.h"
#include "uart.h"

typedef struct Position {
    uint8_t x, y;
} pos;

//=============USER FUNCTIONS=============

void PosMoveToPos(pos *from, pos *to) {
    UartSendString("\033[");
    if (to->x / 10) UartSendChar(to->x / 10 + '0');
    UartSendChar(to->x % 10 + '0');
    UartSendChar(';');
    if (to->y / 10) UartSendChar(to->y / 10 + '0');
    UartSendChar(to->y % 10 + '0');
    UartSendChar('H');
    from->x = to->x;
    from->y = to->y;
//    while (from->x != to.x) {
//        if (from->x < to.x) {
//            UartSendEsc("[C");
//            from->x++;
//        } else {
//            UartSendEsc("[D");
//            from->x--;
//        }
//    }
//    while (from->y != to.y) {
//        if (from->y < to.y) {
//            UartSendEsc("[A");
//            from->y++;
//        } else {
//            UartSendEsc("[B");
//            from->y--;
//        }
//    }
}

void PosMoveToNum(pos *from, uint8_t x, uint8_t y) {
    UartSendString("\033[");
    if (y / 10) UartSendChar(y / 10 + '0');
    UartSendChar(y % 10 + '0');
    UartSendChar(';');
    if (x / 10) UartSendChar(x / 10 + '0');
    UartSendChar(x % 10 + '0');
    UartSendChar('H');
    from->x = x;
    from->y = y;
//    while (from->x != x) {
//        if (from->x < x) {
//            UartSendEsc("[C");
//            from->x++;
//        } else {
//            UartSendEsc("[D");
//            from->x--;
//        }
//    }
//    while (from->y != y) {
//        if (from->y < y) {
//            UartSendEsc("[A");
//            from->y++;
//        } else {
//            UartSendEsc("[B");
//            from->y--;
//        }
//    }
}

void PosMove(pos *cur, int8_t x, int8_t y) {
    PosMoveToNum(cur, cur->x+x, cur->y+y);
//    if (x > 0) {
//        for (uint8_t i = 0; i < x; i++) {
//            UartSendEsc("[C");
//        }
//    } else {
//        for (uint8_t i = 0; i < -x; i++) {
//            UartSendEsc("[D");
//        }
//    }
//    if (y > 0) {
//        for (uint8_t i = 0; i < y; i++) {
//            UartSendEsc("[A");
//        }
//    } else {
//        for (uint8_t i = 0; i < -y; i++) {
//            UartSendEsc("[B");
//        }
//    }
//    cur->x += x;
//    cur->y += y;
}

//End ADC 8 Bit Interfacing Functions

#endif