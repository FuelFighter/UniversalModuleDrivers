/*
 * spi.c
 *
 * Created: 27/03/17 16:25:23
 *  Author: Sondre
 */ 

#include "spi.h"
#include <avr/io.h>

#define DDR_SPI DDRB
#define PIN_MOSI PB2
#define PIN_SCK PB1
#define PIN_SS PB0

void spi_init(spi_prescale_t clock_prescale) {
	// Set MOSI, SS, and SCK output, all others input
	DDR_SPI |= (1 << PIN_MOSI) | (1 << PIN_SCK) | (1 << PIN_SS);
	
	uint8_t SPI2X_val = (uint8_t)((clock_prescale >> 2) & 0b001);
	uint8_t SPR1_val = (uint8_t)((clock_prescale >> 1) & 0b001);
	uint8_t SPR0_val = (uint8_t)((clock_prescale >> 0) & 0b001);

	// Enable SPI, Master, selected prescaling, mode 3 (CPOL = CPHA = 1)
	SPCR = (1 << SPE) | (1 << MSTR) | (SPR1_val << SPR1) | (SPR0_val << SPR0) | (1 << CPOL) | (1 << CPHA);
	SPSR = (SPI2X_val << SPI2X);
}

void spi_trancieve(uint8_t* tx_buffer, uint8_t* rx_buffer, size_t length, bool last_in_transmission) {
	PORTB &= ~(1 << PIN_SS);

	for (size_t i = 0; i < length; i++) {
		SPDR = (tx_buffer != NULL) ? tx_buffer[i] : 0xFF;

		while(!(SPSR & (1 << SPIF)));
		
		if (rx_buffer != NULL) rx_buffer[i] = SPDR;
	}

	if (last_in_transmission) {
		PORTB |= (1 << PIN_SS);
	}
}

void spi_trancieve_tx_then_rx(uint8_t* tx_buffer, size_t tx_length, uint8_t* rx_buffer, size_t rx_length) {
	PORTB &= ~(1 << PIN_SS);

	for (size_t i = 0; i < tx_length; i++) {
		SPDR = tx_buffer[i];
		while(!(SPSR & (1 << SPIF)));
		(void)SPDR;
	}

	for (size_t i = 0; i < rx_length; i++) {
		SPDR = 0xFF;
		while(!(SPSR & (1 << SPIF)));
		rx_buffer[i] = SPDR;
	}

	PORTB |= (1 << PIN_SS);
}
