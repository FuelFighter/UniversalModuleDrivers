//  include  header  files

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "can.h"
#include "uart.h"
#include "can_uart.h"



#ifndef F_CPU
	#define F_CPU 8000000UL // 8 MHz clock speed
#endif



int main(void)
{
	uart_init(BAUD_500000,DISABLED); // 8n1 transmission is set as default
	can_uart_init(UART_BOTH);
	cli();
	can_init(0,0);
	sei();
	CanMessage_t message; //define the message with type "CanMessage_t" which is a struct declared in can.h and defined in can.c

    DDRB = (1<<DDB7)|(1<<DDB6)|(1<<DDB5); // define the direction
	message.id = 0x10;
	message.length = 2;
	message.data.u8[0] = 0xFF;
	message.data.u8[1] = 0xEE;

		
	while (1){
				uint8_t b = can_send_message(&message);
					
				/*CanMessage_t rx;
				if (can_read_message_if_new(&rx)){
					uint16_t id = rx.id;
					uint8_t DATA = rx.data.u8[0];
					// this just to make the RGB LED blinks
				/*	PORTB =  (0<<PB7)|(0<<PB6)|(0<<PB5); //turn ON the LEDs
					_delay_ms(200); //1 second delay
					PORTB =  (1<<PB7)|(1<<PB6)|(1<<PB5); //turn OFF the LEDs
					_delay_ms(200); //1 second delay
				}
		
			can_to_uart(&rx);
		/*	message.id = 0x120;
			message.length = 2;
			message.data.u8[2] = 0xFF;*/
	

	}
}