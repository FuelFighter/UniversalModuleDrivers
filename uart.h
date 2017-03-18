/*
 * uart.h
 *
 * Created: 18/03/17 13:13:39
 *  Author: Sondre
 */ 


#ifndef UART_H_
#define UART_H_


#define F_CPU 8000000UL


void uart_init();
void uart_tx_char(char c);
void uart_tx_str(char* str);


#endif /* UART_H_ */