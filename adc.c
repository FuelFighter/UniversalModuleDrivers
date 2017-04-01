/*
 * adc.c
 *
 * Created: 18.02.2017 10:41:35
 *  Author: olesot
 */ 

//Written for UM

#include "adc.h"
#include <avr/io.h>

void adc_init(void){
	
	/* Voltage ref AVcc with external capacitor on AREF pin */
	ADMUX |= (1<<REFS0);
	
	/* Select prescaler to 64 --> conversion f= 125kHz */
	ADCSRA |= (1<<ADPS2)|(1<<ADPS2);
	
	/* Enable the ADC */
	ADCSRA |= (1<<ADEN);
}

uint16_t adc_read(adc_channel_t channel){
	
	//Setting channel and type of reading, see enum in adc.h 
	ADMUX &= 0b11100000;
	ADMUX |= (int8_t)channel;	
		
	/* Start the conversion */
	ADCSRA |= (1<<ADSC);
	
	/* Wait for the conversion to complete */
	while(ADCSRA & (1<<ADSC));
	
	return ADC;
}
