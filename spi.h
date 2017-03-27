/*
 * spi.h
 *
 * Created: 27/03/17 16:25:14
 *  Author: Sondre
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	DIV_2 = 0b100,
	DIV_4 = 0b000,
	DIV_8 = 0b101,
	DIV_16 = 0b001,
	DIV_32 = 0b110,
	DIV_64 = 0b010,
	DIV_128 = 0b011
} spi_prescale_t;

void spi_init(spi_prescale_t clock_prescale);

// Transmits the tx buffer and writes the received data to the rx buffer. If either buffer
// is a NULL pointer, it will not be used.
// If last_in_transmission is true, the slave select will be deasserted after the transmission. Set
// this to false if there is more data coming.
void spi_trancieve(uint8_t* tx_buffer, uint8_t* rx_buffer, size_t length, bool last_in_transmission);

// Transmits the tx buffer, ignoring any incoming data, then sends dummy data while reading rx_length
// bytes from the slave. Will deassert the slave select after transmission. tx_buffer and rx_buffer can not
// be NULL pointers!
// It is possible to call spi_trancieve with last_in_transmission = false before calling this function, if
// it is needed to split the transmission across several memory locations.
void spi_trancieve_tx_then_rx(uint8_t* tx_buffer, size_t tx_length, uint8_t* rx_buffer, size_t rx_length);


#endif /* SPI_H_ */