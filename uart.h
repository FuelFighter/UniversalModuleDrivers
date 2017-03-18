/*
 * uart.h
 *
 * Created: 18/03/17 13:13:39
 *  Author: Sondre
 */ 


#ifndef UART_H_
#define UART_H_


#include <stdint.h>


#define F_CPU 8000000UL

#define UART_NO_DATA (-1)

// Initialize UART0 with the given baudrate
void uart0_init (uint32_t baudrate);

// Transmit a single byte with UART0
void uart0_transmit(uint8_t data);

// Read the last received byte by UART0
uint16_t uart0_receive (void);


// Initialize UART1 with the given baudrate
void uart1_init (uint32_t baudrate);

// Transmit a single byte with UART1
void uart1_transmit(uint8_t data);

// Read the last received byte by UART0
uint16_t uart1_receive (void);


#endif /* UART_H_ */