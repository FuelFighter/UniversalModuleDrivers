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

typedef union {
	// Integers and fixed point numbers
	// Example: can_message.data.u8[2] = 0xFF;
	uint8_t u8[8];
	int8_t i8[8];
	uint16_t u16[4];
	int16_t i16[4];
	uint32_t u32[2];
	int32_t i32[2];
	uint64_t u64;
	int64_t i64;
	
	// Floating point numbers
	// Example: can_message.data.f32[1] = 3.1415926535;
	float f32[2];
	double f64;

	// Bitfields
	// Example: can_message.data.bitfield[3].b6 = true;
	struct {
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
	} bitfield[8];
} CanData_t;

typedef struct {
	uint16_t id;
	uint8_t length;
	CanData_t data;
} CanMessage_t;

void can_init(uint16_t accept_mask_id, uint16_t accept_tag_id);

bool can_read_message_if_new(CanMessage_t* message);

bool can_send_message(CanMessage_t* message);


#endif /* CAN_H_ */