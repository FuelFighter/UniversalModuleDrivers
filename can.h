/*
 * can.h
 *
 * Created: 18/03/17 13:12:17
 *  Author: Sondre
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
} CanMessage_t;

void can_init(void);

bool can_read_message(CanMessage_t* message);

bool can_send_message(CanMessage_t* message);


#endif /* CAN_H_ */