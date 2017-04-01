/*
 * adc.c
 *
 * Created: 18.02.2017 10:41:35
 *  Author: olesot
 */ 

//Written for UM

#include "adc.h"


void adc_init(void){
	clear_bit(DDRF, DDF0);
	/* Voltage ref AVcc with external capacitor on AREF pin */
	set_bit(ADMUX, REFS0);
	/* Select prescaler to 64 --> conversion f= 125kHz */
	set_bit(ADCSRA, ADPS2);
	set_bit(ADCSRA, ADPS1);
	/* Enable the ADC */
	set_bit(ADCSRA, ADEN);
	/* Turn on the ADC Conversion Complete Interrupt */
	//set_bit(ADCSRA, ADIE);	
}

void adc_enable(void){
	//Enables ADC, turn off before go into sleep mode
	set_bit(ADCSRA, ADEN);
	
}

void start_conv(void){
	set_bit(ADCSRA, ADSC);
}

void adc_disable(void){
	clear_bit(ADCSRA, ADEN);
}

uint8_t adc_read(uint8_t channel){
	
		
	/* Start the conversion */
	set_bit(ADCSRA, ADSC);
	/* Wait for the conversion to complete */
	while(test_bit(ADCSRA, ADSC));
	
	unsigned int full_value = 0;
	full_value = ADC;
	if(full_value > 255)
		full_value = 255;
	else if(full_value < 0)
		full_value = 0;
	return full_value;
}
