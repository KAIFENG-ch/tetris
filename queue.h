/* 
 * File:        queue.h
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

#ifndef _QUEUE__H_
#define _QUEUE__H_

#include "type.h"

//typedef struct QueueNode {
//    struct QueueNode* next;
//    uint8_t data;
//} QNode;
//
//typedef struct Queue {
//    QNode* tail;
//    QNode* head;
//} Queue;

typedef struct Queue {
    uint16_t _data;
//    uint8_t head;
//    uint8_t tail;
    uint8_t ht;
}queue;

//=============USER FUNCTIONS=============

void QueueInit(queue *q) {
    q->_data = 0;
//    q->head = 0;
//    q->tail = 0;
    q->ht = 0;
}

uint8_t _next(uint8_t n) {
    n++;
    if (n > 15) return 0;
    return n;
}

uint8_t QueueLength(queue *q) {
//    int8_t length = q->tail - q->head;
    int8_t length = (q->ht & 15) - (q->ht>>4);
    return length >= 0 ? length : length + 16;
}

uint8_t QueuePush(queue *q, uint8_t i) {
    if (QueueLength(q) < 15) {
        q->_data &= ~(1 << (q->ht&15));
        q->_data |= (i & 1) << (q->ht&15);
        q->ht = _next(q->ht & 15) + (q->ht&15);
//        q->_data &= ~(1 << q->tail);
//        q->_data |= (i & 1) << q->tail;
//        q->tail = _next(q->tail);
        return 1;
    }
    return 0;
}

uint8_t QueuePop(queue *q) {
    if (QueueLength(q) == 0) return 2;
    uint8_t ret = (q->_data & 1 << (q->ht>>4)) > 0 ? 1 : 0;
    q->ht = (_next((q->ht>>4)) << 4) + (q->ht&15);
//    uint8_t ret = (q->_data & 1 << q->head) > 0 ? 1 : 0;
//    q->head = _next(q->head);
    return ret;
}

//End ADC 8 Bit Interfacing Functions

#endif