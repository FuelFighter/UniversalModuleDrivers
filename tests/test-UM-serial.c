//  include  header  files

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/boot.h>

#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "can.h"
#include "uart.h"
#include "can_uart.h"



#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif

#define USE_USART0


int main(void)
{
	
	can_init(0,0); //this function is to initialize the CAN and its arguments are ID mask and ID tag and when both of them are zero this means that it accepts all the IDs
	CanMessage_t message; //define the message with type "CanMessage_t" which is a struct declared in can.h and definned in can.c
	
	message.id = 0x00;
	message.length = 2;
	message.data.u8[0] = 0xaa;
	message.data.u8[1] = 0xbb;
	
	DDRB = (1<<DDB7)|(1<<DDB6)|(1<<DDB5); // define the direction
	//can_send_message(&message);
	uart_init(BAUD_500000,DISABLED); //initialize the UART port there are two arguments one for each UART where you define the baud rate or disable if you don't want to use it
	//can_uart_init(UART_BOTH);
	//can_read_message_if_new(&message);

	unsigned char c[8] = "Hello\n"; // test string
		sei();

	while (1){
			//can_to_uart(&message);
			
			
			uart_transmit_str( UART0_USB, &c ); // this function sends a string over the UART port

			PORTB =  (0<<PB7)|(0<<PB6)|(0<<PB5); //turn ON the LEDs
			_delay_ms(1000); //1 second delay
			PORTB =  (1<<PB7)|(1<<PB6)|(1<<PB5); //turn OFF the LEDs
			_delay_ms(1000); //1 second delay


			
			

	}
	
	
}
