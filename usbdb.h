/*
 * usbdb.h
 *
 * Created: 16.03.2017 20:57:10
 *  Author: Ultrawack
 */ 


#ifndef USBDB_H_
#define USBDB_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

void usbdbg_init();

int usbdbg_tx_char(char c, FILE* stream);

#endif /* USBDB_H_ */