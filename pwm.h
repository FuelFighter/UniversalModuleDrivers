/*
 * pwm.h
 *
 * Created: 24.03.2017 16:13:39
 *  Author: Ultrawack
 */ 

#include <avr/io.h>

#ifndef PWM_H_
#define PWM_H_


typedef enum {
	PWM_PB4,
	PWM_PE3,
	PWM_PE4,
	PWM_PE5
}pwmPin_t;

typedef enum {
	PWM_T2,
	PWM_T3
}pwmTimer_t;

typedef enum {
	DISABLE,
	SCALE_1,
	SCALE_8,
	SCALE_32_T2_ONLY,
	SCALE_64,
	SCALE_128_T2_ONLY,
	SCALE_256,
	SCALE_1024 
}pwmPrescale_t;

// FAST PWM - mode for t2 and t3, 1x prescale, top at 0xFF for t3 and 0xFF t2
void pwm_init(void);

void pwm_set_duty_cycle(pwmPin_t pin, uint16_t dutyCycle);

void pwm_set_top_t3(uint16_t top);

void pwm_set_prescale(pwmPrescale_t scale, pwmTimer_t timer);


#endif /* PWM_H_ */