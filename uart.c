/*
 * uart.c
 *
 * Created: 14.03.2017 20.51.17
 *  Author: mariu
 */

#include "uart.h"
#include <avr/io.h>

// UART0
void uart0_init (uint32_t baudrate) {
	// Configure pin and port
	DDRE |= (1 << PE1);
	DDRE &= ~(1 << PE0);
	
	// Set Asynchronous Mode, 8bit size
	UCSR0C &= ~(1 << UMSEL0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	
	UBRR0 = F_CPU / (16 * baudrate) - 1;
	
	// Enable transmit and receive
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

uint16_t uart0_receive (void) {
	// Check if there is data in receive buffer
	if ((UCSR0A & (1 << RXC0))) {
		// Return data from buffer
		return UDR0;
	}

	return UART_NO_DATA;
}

void uart0_transmit(uint8_t data) {
	// Wait for messages to finish transmitting
	while (!(UCSR0A & (1 << UDRE0))) { };
	UDR0 = data;
}

// UART1
void uart1_init (uint32_t baudrate) {
	// Configure pin and port
	DDRD |= (1 << PD3);
	DDRD &= ~(1 << PD2);
	
	// Set Asynchronous Mode, 8bit size
	UCSR1C &= ~(1 << UMSEL1);
	UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11);
	
	UBRR1 = F_CPU / (16 * baudrate) - 1;
	
	// Enable transmit and receive
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
}

uint16_t uart1_receive (void) {
	// Check if there is data in receive buffer
	if ((UCSR1A & (1 << RXC1))) {
		// Return data from buffer
		return UDR1;
	}

	return UART_NO_DATA;
}

void uart1_transmit(uint8_t data) {
	// Wait for messages to finish transmitting
	while (!(UCSR1A & (1 << UDRE1))) { };
	UDR1 = data;
}
