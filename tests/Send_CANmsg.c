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
	cli();
	can_init(0,0);
	sei();
	CanMessage_t message; //define the message with type "CanMessage_t" which is a struct declared in can.h and defined in can.c

    DDRB = (1<<DDB7)|(1<<DDB6)|(1<<DDB5); // define the direction
	
  message.id = 0x10; // define the ID of the message
	message.length = 2; // define the length of the message
	message.data.u8[0] = 0xFF; // the messsage data
	message.data.u8[1] = 0xEE;

		
	while (1){
				uint8_t b = can_send_message(&message); // this function sends the message through the CAN bus
					
			
	

	}
}
