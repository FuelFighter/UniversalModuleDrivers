/*
 * adc.h
 *
 * Created: 18.02.2017 10:41:48
 *  Author: olesot
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

typedef enum {
	CH_ADC0 = 0b00000,
	CH_ADC1 = 0b00001,
	CH_ADC2 = 0b00010,
	CH_ADC3 = 0b00011,
	CH_ADC4 = 0b00100,
	CH_ADC5 = 0b00101,
	CH_ADC6 = 0b00110,
	CH_ADC7 = 0b00111,
	//CH_DIFF_0_0_10X = 0b01000,
	CH_DIFF_1_0_10X = 0b01001,
	//CH_DIFF_0_0_200X = 0b01010,
	CH_DIFF_1_0_200X = 0b01011,
	//CH_DIFF_2_2_10X = 0b01100,
	CH_DIFF_3_2_10X = 0b01101,
	//CH_DIFF_2_2_200X = 0b01110,
	CH_DIFF_3_2_200X = 0b01111,
	CH_DIFF_0_1 = 0b10000,
	//CH_DIFF_1_1 = 0b10001,
	CH_DIFF_2_1 = 0b10010,
	CH_DIFF_3_1 = 0b10011,
	CH_DIFF_4_1 = 0b10100,
	CH_DIFF_5_1 = 0b10101,
	CH_DIFF_6_1 = 0b10110,
	CH_DIFF_7_1 = 0b10111,
	CH_DIFF_0_2 = 0b11000,
	CH_DIFF_1_2 = 0b11001,
	//CH_DIFF_2_2 = 0b11010,
	CH_DIFF_3_2 = 0b11011,
	CH_DIFF_4_2 = 0b11100,
	CH_DIFF_5_2 = 0b11101,
	CH_CONST_1V1 = 0b11110,
	CH_CONST_0V = 0b11111
} adc_channel_t;


void adc_init(void);

uint16_t adc_read(adc_channel_t channel);

#endif /* ADC_H_ */