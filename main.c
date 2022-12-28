/* 
 * File:        main.c
 * Modified by: rsooio
 *
 * Modified on 8 Dec 2022, 1:23
 */

// CONFIG
#pragma config FOSC = XT    // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF   // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF  // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF  // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF    // Low-Voltage (Single-Supply) In-Circuit Serial
// Programming Enable bit (RB3 is digital I/O, HV
// on MCLR must be used for programming)
#pragma config CPD = OFF    // Data EEPROM Memory Code Protection bit
//(Data EEPROM code protection off)
#pragma config WRT = OFF    // Flash Program Memory Write Enable bits (Write
// protection off; all program memory may be
// written to by EECON control)
#pragma config CP = OFF     // Flash Program Memory Code Protection bit
//(Code protection off)

#include <xc.h>             // Include standard PIC library
#include "type.h"
#include "uart.h"
#include "led.h"
#include "queue.h"
#include "adc.h"
#include "ui.h"
#include "pos.h"
#include "tetris.h"

#ifndef _XTAL_FREQ
// Unless already defined assume 4MHz system frequency
// This definition is required to calibrate the delay
// functions, __delay_us() and  __delay_ms()
#define _XTAL_FREQ 4000000
#endif

queue randset;
tetris game;
uint8_t key[2];
uint8_t flag = 0;

void setup(void);

void main(void) {
    setup();
    UIPageMain();
    UIPageMainToGame();
    TetrisInit(&game, &randset);
    _drawShape(&game, game.shape, game.x, game.y);
    for (;;) {
    }
}

void setup(void) {
    //    T1CON = 0x31;
    //    TMR1IE = 1;
    INTCON |= 0xc0;
    ADIE = 1;
    RCIE = 1;
    UartInit();
    LedInit();
    AdcInit();
    QueueInit(&randset);
    AdcGo();
}

void __interrupt()
isr(void) {
    //    if (TMR1IF) {
    //        TMR1IF = 0;
    //        TMR1H = 0x7a;
    //        TMR1L = 0x12;
    //        if (LedGetGrn())
    //            LedSetRed(!LedGetRed());
    //        LedSetGrn(!LedGetGrn());
    //    }
    if (ADIF) {
        ADIF = 0;
        QueuePush(&randset, AdcVoltL() >> 6);
        if (QueueLength(&randset) < 15) AdcGo();
    }
    if (RCIF) {
        RCIF = 0;
        uint8_t rc = RCREG;
        if (key[0] == 0x1b) {
            if (key[1] == '[') {
                if (rc == 'A') {
                    TetrisRotate(&game);
//                    UartSendChar(0x1b);
//                    UartSendString("[A");
                } else if (rc == 'B') {
                    TetrisNext(&game);
//                    UartSendChar(0x1b);
//                    UartSendString("[B");
                } else if (rc == 'C') {
                    TetrisMove(&game, 1);
//                    UartSendChar(0x1b);
//                    UartSendString("[C");
                } else if (rc == 'D') {
                    TetrisMove(&game, 0);
//                    UartSendChar(0x1b);
//                    UartSendString("[D");
                }
                key[1] = 0;
                key[0] = 0;
            } else {
                if (rc == '[') {
                    key[1] = '[';
                } else {
                    
                }
                if (rc != '[') {
                    key[0] = 0;
                }
            }
        } else {
            if (rc == 0x1b) {
                key[0] = 0x1b;
            }
//            for (uint8_t i = 0; i < 8; i++) {
//                UartSendChar((rc >> (7 - i) & 1) + '0');
//            }
//            UartSendString("\n\r");
//            UartSendChar(rc);
            if (rc == '\r') {
                if ((flag & 1) == 0) {
                    UIClearGame();
                    flag |= 1;
                }
            } else if (rc == 's') {
//                _drawShape(game, game->shape, game->x, game->y);
            } else if (rc == 'd') {
//                UartCurSet(1, 1);
//                for (uint8_t i = 0; i < 8; i++) {
//                    UartSendChar((game.x >> (7 - i) & 1) + '0');
//                }
//                UartSendString("\r\n");
//                for (uint8_t i = 0; i < 8; i++) {
//                    UartSendChar((game.y >> (7 - i) & 1) + '0');
//                }
//                UartSendString("\r\n");
//                for (uint8_t i = 0; i < 8; i++) {
//                    UartSendChar((game.shape >> (7 - i) & 1) + '0');
//                }
//                UartSendString("\r\n");
//                for (uint8_t i = 0; i < 8; i++) {
//                    UartSendChar((game.shapeNext >> (7 - i) & 1) + '0');
//                }
            }
            //        if (RCREG == 'a') {
            //            AdcGo();
            //        }
            //        if (RCREG == 's') {
            //            for (uint8_t i = 0; i < 8; i++) {
            //                UartSendChar((randset.head >> (7 - i) & 1) + '0');
            //            }
            //            UartSendString("\n\r");
            //            for (uint8_t i = 0; i < 8; i++) {
            //                UartSendChar((randset.tail >> (7 - i) & 1) + '0');
            //            }
            //            UartSendString("\n\r");
            //            for (uint8_t i = 0; i < 16; i++) {
            //                UartSendChar((randset._data >> (15 - i) & 1) + '0');
            //            }
            //            UartSendString("\n\r");
            //        }
            //        if (RCREG == 'z') {
            //            uint8_t pop = QueuePop(randset);
            //            if (pop != 2) {
            //                UartSendChar(pop + '0');
            //            }
            //        }
        }
    }
}
