/*
 * IncFile1.h
 *
 * Created: 3/5/2018 6:09:15 PM
 *  Author: Jørgen
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "can.h"
#include "uart.h"

void can_uart_init(uart_channel_t channel);
uint8_t uart_to_can_if_new(CanMessage_t * txFrame);
void can_to_uart(CanMessage_t * rxFrame);

#endif /* INCFILE1_H_ */