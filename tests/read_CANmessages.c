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
	uart_init(BAUD_500000,DISABLED); // initializing the uart
	can_uart_init(UART_BOTH);
	cli(); // resting can registers
	can_init(0,0);
	sei();// set the interrupts
	

		
	while (1){
					
				CanMessage_t rx;
				if (can_read_message_if_new(&rx)){ // recive the message
					uint16_t id = rx.id;
					uint8_t DATA = rx.data.u8[0];
					// this just to make the RGB LED blinks, no functional reason
					PORTB =  (0<<PB7)|(0<<PB6)|(0<<PB5); //turn ON the LEDs
					_delay_ms(200); //1 second delay
					PORTB =  (1<<PB7)|(1<<PB6)|(1<<PB5); //turn OFF the LEDs
					_delay_ms(200); //1 second delay
				}
		
			can_to_uart(&rx); //send the recieved message through the serial port
		
	

	}
}
