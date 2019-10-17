/*
 * can_uart.c
 *
 * Created: 3/5/2018 6:14:03 PM
 *  Author: Jørgen
 */ 


#include "can_uart.h"
#include <stdio.h>

#define CAN_TX_BUFFER_SIZE 64
#define CAN_RX_BUFFER_SIZE 32
#define CAN_TX_BUFFER_MASK (CAN_TX_BUFFER_SIZE-1)
#define CAN_RX_BUFFER_MASK (CAN_RX_BUFFER_SIZE-1)

static unsigned char CAN_TX_BUFFER[CAN_TX_BUFFER_SIZE];
static unsigned char CAN_RX_BUFFER[CAN_RX_BUFFER_SIZE];
static volatile uint8_t tx_head;
static volatile uint8_t tx_tail;
static volatile uint8_t new_can_from_uart;
static volatile uart_channel_t mode;

uint8_t parseCanFromUART(CanMessage_t * txFrame);
uint8_t ascii_to_dec(char c);

void can_uart_init(uart_channel_t channel)
{
	mode = channel;
}

uint8_t uart_to_can_if_new(CanMessage_t * txFrame)
{
	if (mode == UART_BOTH) return 0;
	
	uint8_t tmphead;
	
	while (uart_has_new_message(mode))
	{
		unsigned char data = uart_receive(mode);
		
		if (data == '\n')
		{
			new_can_from_uart++;
		} else {
			CAN_TX_BUFFER[tx_head] = data;
		}
		tmphead = (tx_head + 1) & CAN_TX_BUFFER_MASK;
		tx_head = tmphead;
	}
	
	if (!new_can_from_uart) return 0;
	
	new_can_from_uart--;
	
	return parseCanFromUART(txFrame);
}

uint8_t parseCanFromUART(CanMessage_t * txFrame)
{
	uint16_t canID = 0;
	uint8_t canLength = 0;
	uint8_t canData = 0;
		
	while (CAN_TX_BUFFER[tx_tail] != '[')
	{
		if (tx_tail == tx_head) return 0;
			
		tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
	}

	//Skip [
	tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
		
	//Add MSB of ID
	canID += (ascii_to_dec(CAN_TX_BUFFER[tx_tail])<<8);
	tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
		
	//Add next bit of ID
	canID += (ascii_to_dec(CAN_TX_BUFFER[tx_tail])<<4);
	tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
		
	//Add LSB of ID
	canID += ascii_to_dec(CAN_TX_BUFFER[tx_tail]);
	tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
		
	//Skip :
	tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
		
	//Add length
	canLength = ascii_to_dec(CAN_TX_BUFFER[tx_tail]);
	tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
		
	//Skip :
	tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
		
	//Fill in data
	for (uint8_t byte = 0; byte < canLength; byte++)
	{
		canData = (ascii_to_dec(CAN_TX_BUFFER[tx_tail]) << 4);
		tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
			
		canData |= ascii_to_dec(CAN_TX_BUFFER[tx_tail]);
		tx_tail = (tx_tail + 1) & CAN_TX_BUFFER_MASK;
			
		txFrame->data.u8[byte] = canData;
	}
		
	//Finish adding the CAN information
	txFrame->id = canID;
	txFrame->length = canLength;
		
	//Send the CAN message
	can_send_message(txFrame);
	
	return 1;
}

void can_to_uart(CanMessage_t * rxFrame)
{
	uint8_t index = 0;
	index += sprintf((char*)&CAN_RX_BUFFER[index],"[%03X:%d:", rxFrame->id, rxFrame->length);
	for (int byte = 0; byte < rxFrame->length; byte++)
	{
		index += sprintf((char*)&CAN_RX_BUFFER[index], "%02X", rxFrame->data.u8[byte]);
	}
	sprintf((char*)&CAN_RX_BUFFER[index],"]\n");
	uart_transmit_str(mode,CAN_RX_BUFFER);
}


uint8_t ascii_to_dec(char c)
{
	uint8_t dec = 0;
	
	if ((c >= '0') && (c <= '9'))
	{
		dec = c - '0';
		
	} else if ((c >= 'A') && (c <= 'F'))
	{
		dec = c - 'A' + 10;
		
	} else if ((c >= 'a') && (c <= 'f'))
	{
		dec = c - 'a' + 10;
		
		} else {
		dec = 0;
	}
	
	return (uint8_t)dec;
}