/*
 * uart.h
 *
 * Created: 18/03/17 13:13:39
 *  Author: Jorgejac
 */ 

// UART Driver for the Fuel Fighter Universal Module 

#ifndef UART_H_
#define UART_H_
typedef enum {
	DISABLED = -1,
	BAUD_300 = 0x682,
	BAUD_600 = 0x340,
	BAUD_1200 = 0x1A0,
	BAUD_2400 = 0x0CF,
	BAUD_4800 = 0x067,
	BAUD_9600 = 0x033,
	BAUD_19200 = 0x019,
	BAUD_38400 = 0x00C,
	BAUD_250000 = 0x001,
	BAUD_500000 = 0x000
} baud_t;

typedef enum
{
	UART0_USB = 0,
	UART1_HEADER = 1,
	UART_BOTH = 2,
} uart_channel_t;

// Choose baudrate from the baud_t list, if you only need one of the UART, pass DISABLED. Remember to run sei(); after initializing your program.
void uart_init(baud_t ubrr0_usb,baud_t ubrr1_header);

// Transmits a char or a string respectively
void uart_transmit(uart_channel_t channel, unsigned char c);
void uart_transmit_str(uart_channel_t channel, unsigned char * data);

// Receive one byte
unsigned char uart_receive(uart_channel_t channel);
// Returns how many bytes are in the channels receive buffer
int uart_has_new_message(uart_channel_t channel);

// uart_receive should be used with uart_has_new_message() which will return the amount of bytes in the receive buffer, 0 if no messages.
/*
Example of use to read one byte:

	if(uart_has_new_message(UART0_USB))
	{
		data = uart_receive(UART0_USB);
	}
*/

#endif /* UART_H_ */