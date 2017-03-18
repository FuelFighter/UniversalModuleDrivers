/*
 * rgbled.c
 *
 * Created: 18/03/17 13:53:21
 *  Author: Sondre
 */ 

#include "rgbled.h"
#include <avr/io.h>


void rgbled_init() {
	DDRB |= LED_ALL;
	rgbled_turn_off(LED_ALL);
}

void rgbled_turn_off(RgbLedColor_t color) {
	PORTB |= color;
}

void rgbled_turn_on(RgbLedColor_t color) {
	PORTB &= ~color;
}

void rgbled_toggle(RgbLedColor_t color) {
	PORTB ^= color;
}