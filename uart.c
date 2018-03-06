
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"


// UART0 - This one connects to the USB
/* Buffers */
#define UART0_RX_BUFFER_SIZE 32     /* 2,4,8,16,32,64,128 or 256 bytes */
#define UART0_TX_BUFFER_SIZE 32
#define UART0_RX_BUFFER_MASK (UART0_RX_BUFFER_SIZE - 1)
#define UART0_TX_BUFFER_MASK (UART0_RX_BUFFER_SIZE - 1)

/* Static Variables */
static unsigned char UART0_RxBuf[UART0_RX_BUFFER_SIZE];
static volatile unsigned char UART0_RxHead;
static volatile unsigned char UART0_RxTail;
static unsigned char UART0_TxBuf[UART0_TX_BUFFER_SIZE];
static volatile unsigned char UART0_TxHead;
static volatile unsigned char UART0_TxTail;

// UART1 - This one connects to the HEADER
/* Buffers */
#define UART1_RX_BUFFER_SIZE 32     /* 2,4,8,16,32,64,128 or 256 bytes */
#define UART1_TX_BUFFER_SIZE 32
#define UART1_RX_BUFFER_MASK (UART1_RX_BUFFER_SIZE - 1)
#define UART1_TX_BUFFER_MASK (UART1_RX_BUFFER_SIZE - 1)

/* Static Variables */
static unsigned char UART1_RxBuf[UART1_RX_BUFFER_SIZE];
static volatile unsigned char UART1_RxHead;
static volatile unsigned char UART1_RxTail;
static unsigned char UART1_TxBuf[UART1_TX_BUFFER_SIZE];
static volatile unsigned char UART1_TxHead;
static volatile unsigned char UART1_TxTail;

void uart_init(baud_t ubrr0_usb,baud_t ubrr1_header)
{
	//UART0
	if(ubrr0_usb != DISABLED)
	{
		UBRR0H = (unsigned char)(ubrr0_usb>>8);
		UBRR0L = (unsigned char)ubrr0_usb;
	
		UCSR0C = (3 << UCSZ0);	// 8 bit transfer
		UCSR0B |= (1 << TXEN0)|(1 << RXEN0)|(1 << RXCIE0);
	
		UART0_RxHead = 0;
		UART0_RxTail = 0;
		UART0_TxHead = 0;
		UART0_TxTail = 0;
	}
	//UART1
	if(ubrr1_header != DISABLED)
	{
		UBRR1H = (unsigned char)(ubrr1_header>>8);
		UBRR1L = (unsigned char)ubrr1_header;
		
		UCSR1C = (3 << UCSZ0);	// 8 bit transfer
		UCSR1B |= (1 << TXEN1)|(1 << RXEN1)|(1 << RXCIE1);
		
		UART1_RxHead = 0;
		UART1_RxTail = 0;
		UART1_TxHead = 0;
		UART1_TxTail = 0;
	}
}

void uart_transmit(uart_channel_t channel, unsigned char c)
{
	unsigned char tmphead;
	
	switch (channel)
	{
		case UART0_USB:
	
			/* Calculate buffer index */
			tmphead = (UART0_TxHead + 1) & UART0_TX_BUFFER_MASK;
			/* Wait for free space in buffer */
			while (tmphead == UART0_TxTail);
			/* Store data in buffer */
			UART0_TxBuf[tmphead] = c;
			/* Store new index */
			UART0_TxHead = tmphead;
			/* Enable UDRE interrupt */
			UCSR0B |= (1<<UDRIE0);
			break;
		
		case UART1_HEADER:
		
			/* Calculate buffer index */
			tmphead = (UART1_TxHead + 1) & UART1_TX_BUFFER_MASK;
			/* Wait for free space in buffer */
			while (tmphead == UART1_TxTail);
			/* Store data in buffer */
			UART1_TxBuf[tmphead] = c;
			/* Store new index */
			UART1_TxHead = tmphead;
			/* Enable UDRE interrupt */
			UCSR1B |= (1<<UDRIE1);
			break;
		
		case UART_BOTH:
	
			uart_transmit(UART0_USB,c);
			uart_transmit(UART1_HEADER,c);
			break;
		
		default:
			break;
	}
}

void uart_transmit_str(uart_channel_t channel, unsigned char * data)
{
	for (size_t byte = 0; byte < strlen(data); byte++)
	{
		uart_transmit(channel,data[byte]);
	}
}

unsigned char uart_receive(uart_channel_t channel)
{
	unsigned char tmptail;
	
	switch (channel)
	{
		case UART0_USB:
		
			/* Wait for incoming data */
			if(UART0_RxHead == UART0_RxTail) break;
			/* Calculate buffer index */
			tmptail = (UART0_RxTail + 1) & UART0_RX_BUFFER_MASK;
			/* Store new index */
			UART0_RxTail = tmptail;
			/* Return data */
			return UART0_RxBuf[tmptail];
			
		case UART1_HEADER:
			
			/* Wait for incoming data */
			if(UART1_RxHead == UART1_RxTail) break;
			/* Calculate buffer index */
			tmptail = (UART1_RxTail + 1) & UART1_RX_BUFFER_MASK;
			/* Store new index */
			UART1_RxTail = tmptail;
			/* Return data */
			return UART1_RxBuf[tmptail];
			
		case UART_BOTH:
			/* Not implemented for UART_BOTH*/
			
		default:
			return '0';
	}
	return '0';
}

int uart_has_new_message(uart_channel_t channel)
{
	switch (channel)
	{
		case UART0_USB:
			return (UART0_RxHead-UART0_RxTail);
		case UART1_HEADER:
			return (UART1_RxHead-UART1_RxTail);
		case UART_BOTH:	
		default:
			break;
	}
	return 0;
}

ISR(USART0_RX_vect)
{
	unsigned char data;
	unsigned char tmphead;
	
	/* Read the received data */
	data = UDR0;
	/* Calculate buffer index */
	tmphead = (UART0_RxHead + 1) & UART0_RX_BUFFER_MASK;
	/* Store new index */
	UART0_RxHead = tmphead;

	if (tmphead == UART0_RxTail) {
		/* ERROR! Receive buffer overflow */
	}
	/* Store received data in buffer */
	UART0_RxBuf[tmphead] = data;
}

ISR(USART0_UDRE_vect)
{
	unsigned char tmptail;

	/* Check if all data is transmitted */
	if (UART0_TxHead != UART0_TxTail) 
	{
		/* Calculate buffer index */
		tmptail = ( UART0_TxTail + 1 ) & UART0_TX_BUFFER_MASK;
		/* Store new index */
		UART0_TxTail = tmptail;
		/* Start transmission */
		UDR0 = UART0_TxBuf[tmptail];
	} else {
		/* Disable UDRE interrupt */
		UCSR0B &= ~(1<<UDRIE0);
	}
}

ISR(USART1_RX_vect)
{
	unsigned char data;
	unsigned char tmphead;
	
	/* Read the received data */
	data = UDR1;
	/* Calculate buffer index */
	tmphead = (UART1_RxHead + 1) & UART1_RX_BUFFER_MASK;
	/* Store new index */
	UART1_RxHead = tmphead;

	if (tmphead == UART1_RxTail) {
		/* ERROR! Receive buffer overflow */
	}
	/* Store received data in buffer */
	UART1_RxBuf[tmphead] = data;
}

ISR(USART1_UDRE_vect)
{
	unsigned char tmptail;

	/* Check if all data is transmitted */
	if (UART1_TxHead != UART1_TxTail)
	{
		/* Calculate buffer index */
		tmptail = ( UART1_TxTail + 1 ) & UART1_TX_BUFFER_MASK;
		/* Store new index */
		UART1_TxTail = tmptail;
		/* Start transmission */
		UDR1 = UART1_TxBuf[tmptail];
	} else {
		/* Disable UDRE interrupt */
		UCSR1B &= ~(1<<UDRIE1);
	}
}