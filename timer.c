/*
 * timer.c
 *
 * Created: 01/04/17 15:46:48
 *  Author: Sondre
 */ 

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

#define F_CPU 8000000UL	
#define PRESCALE 8				//Divide clock by this value
#define FREQ 10000				//Increment frequency
#define COMPARE_VAL (F_CPU/(PRESCALE*FREQ))-1	//Compare value for chosen frequency
#define MICROSECOND_INCREMENT 1000000/FREQ	//Increment in microseconds used every interrupt

#define NUMBER_OF_TIMERS 8

static bool timer_enabled[NUMBER_OF_TIMERS];
static uint32_t elapsed_microseconds[NUMBER_OF_TIMERS];

void timer_init() {
	// Configure timer with normal mode
	TCCR0A = 0;
	// Enable overflow interrupt
	TIMSK0 = (1 << OCIE0A);
	// Enable timer with CLK_io/8
	TCCR0A |= (1<<CS01);
	OCR0A = COMPARE_VAL;
}

void timer_start(timer_t timer) {
	elapsed_microseconds[timer] = 0;
	timer_enabled[timer] = true;
}

void timer_stop(timer_t timer) {
	timer_enabled[timer] = false;
}

uint16_t timer_elapsed_ms(timer_t timer) {
	return elapsed_microseconds[timer] / 1000;
}

ISR(TIMER0_COMP_vect) {
	for (int t = 0; t < NUMBER_OF_TIMERS; t++) {
		if (timer_enabled[t]){
			elapsed_microseconds[t] += MICROSECOND_INCREMENT;
		}
	}
}
