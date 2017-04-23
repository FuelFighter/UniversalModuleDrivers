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

#define BRAKE_CAN_ID			0x110
#define ENCODER_CAN_ID			0x220
#define STEERING_WHEEL_CAN_ID	0x230
#define DASHBOARD_CAN_ID		0x310
#define BMS_CELL_V_1_4_CAN_ID	0x440
#define BMS_CELL_V_5_7_CAN_ID	0x441
#define BMS_CELL_V_8_12_CAN_ID	0x442
#define BMS_CELL_TEMP_CAN_ID	0x443
#define BMS_VOLT_CURRENT_CAN_ID 0x444
#define BMS_STATUS_CAN_ID		0x448
#define BMS_ERROR_CAN_ID		0x449
#define MOTOR_1_STATUS_CAN_ID	0x450
#define MOTOR_2_STATUS_CAN_ID	0x460
#define FRONT_LIGHTS_CAN_ID		0x470
#define BACK_LIGHTS_CAN_ID		0x480

typedef struct {
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
} CanMessage_t;

void can_init(uint16_t accept_mask_id, uint16_t accept_tag_id);

bool can_read_message_if_new(CanMessage_t* message);

bool can_send_message(CanMessage_t* message);


#endif /* CAN_H_ */