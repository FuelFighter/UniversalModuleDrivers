/*
 * pwm.c
 *
 * Created: 24.03.2017 15:38:18
 *  Author: Jørgen Jackwitz
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>

//sets bit "bit" in register "reg"
#define set_bit(reg, bit)		(reg |= (1<<bit))
//same as set_bit only clears all other bits
#define set_bit_ex(reg, bit)	(reg = (1<<bit))
//clears bit "bit" in register "reg"
#define clear_bit(reg, bit)		(reg &=~ (1<<bit))
//tests if bit "bit" is set in register "reg"
#define test_bit(reg, bit)		(reg & (1<<bit))
//toggles bit "bit" in register "reg"
#define toggle_bit(reg, bit)	(reg ^= (1<<bit))

void pwm_init(void){
	
	//OC3A & B pin as output
	PORTE &= ~(1<<PE3);
	DDRE |= (1<<PE3);
	PORTE &= ~(1<<PE4);
	DDRE |= (1<<PE4);
	
	//fast pwm, mode 14, TOP at ICR
	set_bit(TCCR3B, WGM33);
	set_bit(TCCR3B, WGM32);
	set_bit(TCCR3A,WGM31);
	clear_bit(TCCR3A,WGM30);

	// Set low on compare match for 3A&B
	set_bit(TCCR3A, COM3A1);
	clear_bit(TCCR3A, COM3A0);
	set_bit(TCCR3A, COM3B1);
	clear_bit(TCCR3A, COM3B0);
	
	//Set prescale clk/1 for timer 3
	clear_bit(TCCR3B, CS32);
	clear_bit(TCCR3B, CS31);
	set_bit(TCCR3B, CS30);
	
	//Set top value for timer 3
	ICR3 = 0xFF;
	
	//Set off 3A&B
	OCR3A = 0x0000;
	OCR3B = 0x0000;
}
