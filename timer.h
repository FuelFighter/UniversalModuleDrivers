/*
 * timer.h
 *
 * Created: 01/04/17 15:46:57
 *  Author: Sondre
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

typedef enum {
	TIMER0 = 0,
	TIMER1,
	TIMER2,
	TIMER3,
	TIMER4,
	TIMER5,
	TIMER6,
	TIMER7
} timer_t;

void timer_init();

void timer_start(timer_t timer);

void timer_stop(timer_t timer);

uint16_t timer_elapsed_ms(timer_t timer);


#endif /* TIMER_H_ */