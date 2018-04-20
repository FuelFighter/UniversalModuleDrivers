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

void adc_Free_running_init(void){
	
	/* Voltage ref AVcc with external capacitor on AREF pin */
	ADMUX |= (1<<REFS0);
	
	/* Select prescaler to 64 --> conversion f= 125kHz */
	ADCSRA |= (1<<ADPS2);
	
	/* Auto trigger of ADC */
	ADCSRA |= (1<<ADATE);
	
	/* Enable Interrupt of ADC */
	ADCSRA |= (1<<ADIE);
	
	/* Free running mode */
	ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0)); // (initially at 0 so not useful)
	
	/* Enable the ADC */
	ADCSRA |= (1<<ADEN);
	
	/* Start the conversion */
	ADCSRA |= (1<<ADSC);
}

uint16_t adc_Free_running_read(adc_channel_t channel){
	//Setting channel and type of reading, see enum in adc.h
	ADMUX &= 0b11100000;
	ADMUX |= (int8_t)channel;

	return (ADCL+(ADCH<<8));
}

void Set_ADC_Channel(adc_channel_t channel)
{
	channel &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|channel; // clears the bottom 3 bits before ORing
}
