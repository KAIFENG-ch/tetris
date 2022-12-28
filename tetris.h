/* 
 * File:        tetris.h
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

#ifndef _TETRIS__H_
#define _TETRIS__H_

#include "type.h"
#include "queue.h"
#include "adc.h"
#include "pos.h"
#include "uart.h"

#define ForEachBlock(C,X,Y) for (uint16_t p=table[C],bx,by;bx=(uint8_t)(X+p%4),by=(uint8_t)(Y+p/4%4),p;p>>=4)
#define ForEachBlockCustom(C,X,Y,BX,BY) for (uint16_t p=table[C];BX=X+p%4,BY=Y+p/4%4,p;p>>=4)
//#define ForEachUnit(C) for (uint16_t p=table[C],ux,uy;ux=p%4,uy=p/4%4,p;p>>=4)
#define Last(C) ((C)&15)
#define LastEq(C,D) (Last(C)==Last(D))
#define LastEq4(C,D) (LastEq(C,D)||LastEq(C,D>>4)||LastEq(C,D>>8)||LastEq(C,D>>12))
#define TetrisMap(C) (*C)
#define TetrisShape(C) (*(C+1))
#define TetrisShapeNext(C) (*(C+2))
#define TetrisX(C) (*(C+3)
#define TetrisY(C) (*(C+4)

typedef struct Tetris {
    uint8_t map[25], shape, shapeNext, x, y;
    uint16_t nextMap;
    queue *randset;
} tetris;

//=============USER FUNCTIONS=============

const uint16_t table[] = {
    0x4567, 0x26ae, 0xba98, 0xd951,
    0x6540, 0x9521, 0xa654, 0x9851,
    0x6542, 0xa951, 0x8654, 0x9510,
    0x5421, 0xa651, 0x9865, 0x9540,
    0x6510, 0x9652, 0xa954, 0x8541,
    0x6541, 0x9651, 0x9654, 0x9541,
    0xa965
};

uint8_t _randShape(queue *randset) {
    uint8_t shape = 0, rotate = 0;
    for (uint8_t i = 0; i < 3; i++) {
        shape |= (QueuePop(randset) << i);
    }
    AdcGo();
    while (shape == 7) {
        shape <<= 1;
        shape &= 7;
        shape |= (QueuePop(randset) & 1);
    }
    if (shape == 6) return 24;
    for (uint8_t i = 0; i < 2; i++) {
        rotate |= (QueuePop(randset) << i);
    }
    return 4 * shape + rotate;
}

uint8_t _getBlock(tetris *t, uint8_t x, uint8_t y) {
    if (x < 10 && y < 20) {
        uint8_t n = 10 * y + x;
        return (t->map[n / 8] >> (n % 8)) & 1;
    }
    return 0;
}

void _clrBlock(tetris *t, uint8_t x, uint8_t y) {
    if (x < 10 && y < 20) {
        uint8_t n = 10 * y + x;
        t->map[n / 8] &= ~(1 << n % 8);
    }
}

void _setBlock(tetris *t, uint8_t x, uint8_t y) {
    if (x < 10 && y < 20) {
        uint8_t n = 10 * y + x;
        t->map[n / 8] |= (1 << n % 8);
    }
}

void _drawBlock(uint8_t x, uint8_t y) {
    UartCurSet(2 * x + 24, y + 2);
    UartSendString("\xe2\x96\x88\xe2\x96\x88");
}

void _wipeBlock(uint8_t x, uint8_t y) {
    UartCurSet(2 * x + 24, y + 2);
    UartSendString("  ");
}

void _drawShape(tetris *t, uint8_t shape, uint8_t x, uint8_t y) {

    ForEachBlock(shape, x, y) {
        if (bx < 10 && by < 20 && _getBlock(t, bx, by)) {
            _drawBlock(bx, by);
        }
    }
}

void _wipeShape(tetris *t, uint8_t shape, uint8_t x, uint8_t y) {

    ForEachBlock(shape, x, y) {
        if (bx < 10 && by < 20 && !_getBlock(t, bx, by)) {
            _wipeBlock(bx, by);
        }
    }
}

//uint8_t isLineFill(tetris *t, uint8_t line) {
//    uint8_t *data = t->map + line / 4 * 5 + line % 4;
//    switch (line % 4) {
//        case 0:
//            return *data == 0xff && (*(data + 1) >> 6) == 0x03;
//        case 1:
//            return (*data << 2) == 0xfc && (*(data + 1) >> 4) == 0x0f;
//        case 2:
//            return (*data << 4) == 0xf0 && (*(data + 1) >> 2) == 0x3f;
//        case 3:
//            return (*data << 6) == 0xc0 && *(data + 1) == 0xff;
//    }
//}

void _shapeNext(tetris *t) {
    t->shape = t->shapeNext;
    t->x = 3;
    //    if (t->shape == 24 || t->shape % 4 == 2) t->y = -1;
    //    else if (t->shape == 0) t->y = -2;
    //    else t->y = 0;
    if (t->shape % 4 == 0 && t->shape != 24) t->y = -2;
    else if (t->shape == 1 || t->shape == 3) t->y = -4;
    else t->y = -3;
    t->shapeNext = _randShape(t->randset);
    for (uint16_t p = table[t->shape]; p; p >>= 4) t->nextMap &= ~(1 << (p & 15));
    for (uint16_t p = table[t->shapeNext]; p; p >>= 4) t->nextMap |= (1 << (p & 15));
    for (uint16_t p = table[t->shape]; p; p >>= 4) {
        if (!(t->nextMap >> (p & 15))&1) _wipeBlock(p % 4 + 14, p / 4 % 4 + 3);
    }
    for (uint16_t p = table[t->shapeNext]; p; p >>= 4) {
        if ((t->nextMap >> (p & 15))&1) _drawBlock(p % 4 + 14, p / 4 % 4 + 3);
    }
}

void TetrisInit(tetris *t, queue *randset) {
    t->randset = randset;
    t->shapeNext = _randShape(randset);
    _shapeNext(t);
}

void TetrisMove(tetris *t, uint8_t direction) {
    uint8_t x = t->x + (direction ? 1 : -1);
    ForEachBlock(t->shape, t->x, t->y) _clrBlock(t, bx, by);

    ForEachBlock(t->shape, x, t->y) {
        if (bx > 9 || _getBlock(t, bx, by)) break;
        if (p < 16) t->x = x;
    }
    ForEachBlock(t->shape, t->x, t->y) _setBlock(t, bx, by);
    if (t->x != x) return;
    _wipeShape(t, t->shape, t->x - (direction ? 1 : -1), t->y);
    _drawShape(t, t->shape, t->x, t->y);
    UartCurSet(1, 1);
    for (uint8_t i = 0; i < 20; i++) {
        for (uint8_t j = 0; j < 10; j++) {
            UartSendChar(_getBlock(t, j, i) + '0');
        }
        UartCurMove(-10, 1);
    }
}

void TetrisRotate(tetris *t) {
    if (t->shape != 24) {
        ForEachBlock(t->shape, t->x, t->y) _clrBlock(t, bx, by);
        uint8_t shapeRotate = t->shape, dx, flag = 0;
        for (uint8_t i = 0; i < 3; i++) {
            shapeRotate += (shapeRotate % 4 == 3 ? 1 : -3);
            for (dx = 0; dx <= (t->shape < 4 ? 2 : 1); dx = (dx > 0 ? -dx : 1 - dx)) {
                ForEachBlock(shapeRotate, t->x + dx, t->y) {
                    if (bx > 9 || by == 20 || _getBlock(t, bx, by)) break;
                    if (p < 16) flag = 1;
                }
            }
            if (flag) {
                ForEachBlock(shapeRotate, t->x + dx, t->y) _setBlock(t, bx, by);
                _wipeShape(t, t->shape, t->x, t->y);
                t->x += dx;
                t->shape = shapeRotate;
                _wipeShape(t, t->shape, t->x, t->y);
                return;
            }
        }
        ForEachBlock(t->shape, t->x, t->y) _setBlock(t, bx, by);
    }
}

void TetrisNext(tetris *t) {
    uint8_t y = t->y + 1;
    ForEachBlock(t->shape, t->x, t->y) _clrBlock(t, bx, by);
    UartCurSet(12, 1);

    ForEachBlock(t->shape, t->x, y) {
        if (by == 20) break;
        if (_getBlock(t, bx, by)) break;
        if (p < 16) t->y = y;
    }
    ForEachBlock(t->shape, t->x, t->y) _setBlock(t, bx, by);
    if (t->y != y) {

        ForEachBlock(t->shape, t->x, t->y) {
            if ((int8_t) by < 0) {
                //end
                return;
            }
        }
        _shapeNext(t);
        return;
    };
    _wipeShape(t, t->shape, t->x, t->y - 1);
    _drawShape(t, t->shape, t->x, t->y);
    UartCurSet(1, 1);
    for (uint8_t i = 0; i < 20; i++) {
        for (uint8_t j = 0; j < 10; j++) {
            UartSendChar(_getBlock(t, j, i) + '0');
        }
        UartCurMove(-10, 1);
    }
}

//End ADC 8 Bit Interfacing Functions

#endif