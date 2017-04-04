/*
 * usbdb.c
 *
 * Created: 16.03.2017 20:55:52
 *  Author: Ultrawack
 */ 
#include "usbdb.h"
#include <stdlib.h>

void usbdbg_init()
{
	const uint32_t baud_rate = 19200;
	UBRR0 = (F_CPU / 16) / baud_rate - 1;
	UCSR0C = (3 << UCSZ0);	// 8 bit transfer
	UCSR0B |= (1 << TXEN0)|(1<<RXEN0)|(1<<RXCIE0); // Enable receive and transmit
	
	fdevopen(usbdbg_tx_char, NULL);
}

int usbdbg_tx_char(char c, FILE* stream)
{
	while (!(UCSR0A & (1 << UDRE0))) { }; // Wait until data register is empty
	UDR0 = c;
	return 1;
}

char usbdbg_rx_char()
{
	return UDR0;
} 


