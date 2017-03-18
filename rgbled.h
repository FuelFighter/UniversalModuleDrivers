/*
 * rgbled.h
 *
 * Created: 18/03/17 13:53:30
 *  Author: Sondre
 */ 


#ifndef RGBLED_H_
#define RGBLED_H_


typedef enum {
	LED_RED = (1 << 5),
	LED_GREEN = (1 << 6),
	LED_BLUE = (1 << 7),
	LED_ALL = LED_RED | LED_GREEN | LED_BLUE
} RgbLedColor_t;


void rgbled_init();

void rgbled_turn_off(RgbLedColor_t color);

void rgbled_turn_on(RgbLedColor_t color);

void rgbled_toggle(RgbLedColor_t color);


#endif /* RGBLED_H_ */