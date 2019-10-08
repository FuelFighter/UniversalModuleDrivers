/*
 * test.c
 *
 * Created: 10/6/2019 2:33:40 PM
 * Author : motaz
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

/*this function is used to get delay with variable delay value*/
void my_delay_ms(int n) {
	while(n--) {
		_delay_ms(1);
	}
}


int main(void)
{
    /* Replace with your application code */
	
	DDRB = (1<<DDB7)|(1<<DDB6)|(1<<DDB5); // define the direction

    while (1) 
    {
			for (int i =10;i>0; i--)
			{
				
				PORTB = (1<<PB7)|(1<<PB6)|(1<<PB5); //turn OFF the LEDs the output is inverted
				my_delay_ms(1000/i); //1 second delay
				PORTB =  (0<<PB7)|(0<<PB6)|(0<<PB5); //turn ON the LEDs
				my_delay_ms(1000/i); //1 second delay
			}
			
    }
}

