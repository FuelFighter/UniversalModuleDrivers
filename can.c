#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/boot.h>

#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "can.h"

#ifndef F_CPU
#error F_CPU not defined. Define it in project settings.
#endif

#define	CAN_FRAME_DATA_MAX_LENGTH	8
#define	CAN_FRAME_MSG_LENGTH	3
#define CAN_FRAME_SIZE			(CAN_FRAME_DATA_MAX_LENGTH + CAN_FRAME_MSG_LENGTH)


typedef union {
	struct {
		uint32_t rb0		: 1;
		uint32_t filler1	: 1;
		uint32_t rbr		: 1;
		uint32_t filler2	: 1;
		uint32_t filler3	: 1;
		uint32_t id			:11;
		uint32_t length		: 4;
		uint32_t filler4	: 4;
		uint8_t	data[CAN_FRAME_DATA_MAX_LENGTH];
	};
	unsigned char		array[CAN_FRAME_MSG_LENGTH + CAN_FRAME_DATA_MAX_LENGTH];
} can_frame;


#define	NODE_ID	0xff
#define	CRIS_ID 0x000

#define	CAN_BAUD_RATE		500000

/**************************************************************************************************
*   CAN PHY driver delays and LINE delay (unit = nS)
**************************************************************************************************/
#define	TX_DELAY		50			//bus driver delay
#define	RX_DELAY		30			//receiver delay
#define	LINE_DELAY		110			//bus line delay

/**************************************************************************************************
*   Acceptance Filtering,
**************************************************************************************************/
#define ACCPT_MASK_RTR	0
#define ACCPT_TAG_RTR	0
#define ACCPT_MASK_IDE	0
#define ACCPT_TAG_RB0	0
#define ACCPT_TAG_RB1	0			//RB1 in extended (CAN 2.0B) Only

/**************************************************************************************************
*   TX and RX frame buffer sizes
* 	- 1 can frame ~ 11 bytes for CAN2A and 13 bytes for CAN2B
* 	- RX_SIZE * 11 or 13 is aprox rx buffer memory size.
**************************************************************************************************/
#define TX_SIZE			16			//Transmit Buffer Size, can be 2^n where n=0 to 6
#define RX_SIZE			16			//Receiver Buffer Size, can be 2^n where n=0 to 6

/**************************************************************************************************
*   Pre-Processor ONLY! - Do not edit!
**************************************************************************************************/
#define BIT_TIME_NS			(1000000000/CAN_BAUD_RATE)
#define TCSC				(BIT_TIME_NS/16)
#define TPRS				(2*(TX_DELAY+RX_DELAY+LINE_DELAY))
#define	PROPAG_CALC			((TPRS+(TCSC/2-1))/TCSC)			//round at 0.5 instead of 1
#if PROPAG_CALC>7
#define	PROP_SEG		PROPAG_CALC
#else
#define	PROP_SEG		7
#endif
#define	REMAINING			(16-1-(PROP_SEG-1)-1)
#define	PHASE_SEG_1			(REMAINING/2)
#if (REMAINING & 1)											//check if odd or even
#define	PHASE_SEG_2		(PHASE_SEG_1-1)						//phase2 <= phase1
#else
#define	PHASE_SEG_2		PHASE_SEG_1
#endif

#define	SJW_VALUE			1
#define	BRP_VALUE			(F_CPU/CAN_BAUD_RATE/16)

#if (BRP_VALUE > 0x3F)
#warning CAN Baud Rate too LOW
#endif

#if (PHASE_SEG_2 < 2)
#warning Propagation time is too long!
#endif

#if (F_CPU%CAN_BAUD_RATE)
#warning Clock (F_CPU) is not a multipe of CAN Baud rate!
#endif

#if !(TX_SIZE==64 || TX_SIZE==32 || TX_SIZE==16 || TX_SIZE==8 \
|| TX_SIZE==4 || TX_SIZE==2 || TX_SIZE==1)
#warning:Wrong CAN TX Buffer Size
#endif

#if !(RX_SIZE==64 || RX_SIZE==32 || RX_SIZE==16 || RX_SIZE==8 \
|| RX_SIZE==4 || RX_SIZE==2 || RX_SIZE==1)
#warning:Wrong CAN RX Buffer Size
#endif

#define RX_ABS_MASK		0x7F
#define TX_ABS_MASK		0x7F

/**************************************************************************************************
*   Internal Variables
**************************************************************************************************/
static can_frame tx_frames[TX_SIZE];
static can_frame rx_frames[RX_SIZE];
static uint8_t tx_off;
static uint8_t tx_on;
static uint8_t tx_busy;
static uint8_t rx_off;
static uint8_t rx_on;
static volatile uint8_t reset;

/**************************************************************************************************
*   CAN ISR - See 'can.h' Header file for Description
**************************************************************************************************/
ISR(CANIT_vect)
{
	volatile uint8_t mob_status;

	uint8_t mob_interrupts = CANSIT2;

	// TX
	if ((mob_interrupts & (1 << SIT0)) && (CANIE2 & (1 << ENMOB0))) {
		CANPAGE = (0x0 << MOBNB0); //Select TX Mob (Mob0)
		mob_status = CANSTMOB;
		CANSTMOB &= ~(1 << TXOK); //clear MB1, TX interrupt

		if (tx_on != tx_off) {
			unsigned char pos;
			pos = tx_off & (TX_SIZE-1);
			//set ID
			CANIDT4 = tx_frames[pos].array[0];
			CANIDT2 = tx_frames[pos].array[0];
			CANIDT1 = tx_frames[pos].array[1];

			//program data registers - auto increment CANMSG
			CANMSG = tx_frames[pos].data[0];
			CANMSG = tx_frames[pos].data[1];
			CANMSG = tx_frames[pos].data[2];
			CANMSG = tx_frames[pos].data[3];
			CANMSG = tx_frames[pos].data[4];
			CANMSG = tx_frames[pos].data[5];
			CANMSG = tx_frames[pos].data[6];
			CANMSG = tx_frames[pos].data[7];

			//set length and request send
			CANCDMOB = (1 << CONMOB0) | tx_frames[pos].length;
			tx_off++;
		} else {
			tx_busy = 0;
		}
	}
	// RX
	else if ((mob_interrupts & (1 << SIT1)) && (CANIE2 & (1 << ENMOB1))) {
		//Select RX Mob (Mob1)
		CANPAGE = (0x1 << MOBNB0);
		if (((rx_on - rx_off) & RX_ABS_MASK) < RX_SIZE) {
			unsigned char pos;
			pos = rx_on & (RX_SIZE-1);
			//Read length
			rx_frames[pos].length = CANCDMOB & 0x0F;
			//Read ID
			rx_frames[pos].array[0] = (CANIDT2 & 0xE0) | (CANIDT4 & 0x07);
			rx_frames[pos].array[1] = CANIDT1;

			//read data registers - auto increment CANMSG
			rx_frames[pos].data[0] = CANMSG;
			rx_frames[pos].data[1] = CANMSG;
			rx_frames[pos].data[2] = CANMSG;
			rx_frames[pos].data[3] = CANMSG;
			rx_frames[pos].data[4] = CANMSG;
			rx_frames[pos].data[5] = CANMSG;
			rx_frames[pos].data[6] = CANMSG;
			rx_frames[pos].data[7] = CANMSG;
			rx_on++;

			// Reset if reset can message
			if(rx_frames[pos].id == 0x000 && rx_frames[pos].data[0] == 0x03){
				wdt_enable(WDTO_15MS);
				while(1); //wait for watchdog
			}
		}

		// Clear irq
		mob_status = CANSTMOB;
		(void)mob_status;

		CANSTMOB &= ~(1 << RXOK);
		CANCDMOB = (1 << CONMOB1);			//Set Mob 1 as RX

	}
}


void can_init(uint16_t accept_mask_id, uint16_t accept_tag_id) {
	// Reset CAN controller
	CANGCON = (1 << SWRES);

	CANBT1 = (BRP_VALUE - 1) << 1;
	CANBT2 = ((SJW_VALUE - 1) << 5) | ((PROP_SEG - 1) << 1);
	CANBT3 = ((PHASE_SEG_2 - 1) << 4) | ((PHASE_SEG_1 - 1) << 1) | 1;

	CANTIM = 0;
	CANTTC = 0;

	CANHPMOB = 0;
	CANTCON = 0;

	// Switch to Mob 0 access
	CANPAGE = (0x0 << MOBNB0);
	CANSTMOB = 0;

	// Switch to Mob 1 access
	CANPAGE = (0x1 << MOBNB0);
	CANSTMOB = 0;
	CANIDM4 = 0;
	CANIDM2 = (accept_mask_id << 5) & 0xFF;
	CANIDM1 = (accept_mask_id >> 3) & 0xFF;
	CANIDT4 = 0;
	CANIDT2 = (accept_tag_id << 5) & 0xFF;
	CANIDT1 = (accept_tag_id >> 3) & 0xFF;

	// Set Mob 1 as RX
	CANCDMOB = (1 << CONMOB1);

	// Enable Mob 0 and 1
	CANEN2 = (1 << ENMOB1) | (1 << ENMOB0);
	// Enable Mob 0 and 1 Interrupt
	CANIE2 = (1 << ENMOB1) | (1 << ENMOB0);
	// Enable TX and RX interrupt
	CANGIE = (1 << ENIT) | (1 << ENRX) | (1 << ENTX);	

	// Enable CAN controller
	CANGCON = (1 << ENASTB);

	reset = 0;
}

bool can_read_message_if_new(CanMessage_t* message) {
	// Check if there is a new message
	if (rx_on == rx_off) {
		return false;
	}

	// Read the can frame
	can_frame* frame = &rx_frames[(rx_off & (RX_SIZE - 1))];

	message->id = frame->id;
	message->length = frame->length;
	for (int i = 0; i < message->length; i++) {
		message->data.u8[i] = frame->data[i];
	}

	// Advance to next can message
	if (rx_on != rx_off) {
		rx_off++;
	}

	return true;
}

bool can_send_message(CanMessage_t* message) {
	bool result = false;

	CANGIE &= ~(1 << ENIT);

	if (!tx_busy) {
		// Switch to Mob 0 access
		CANPAGE = 0 << 4;

		// Set ID
		CANIDT2 = message->id << 5;
		CANIDT1 = message->id >> 3;

		// Program data registers - auto increment
		for (int i = 0; i < 8; i++) {
			CANMSG = message->data.u8[i];
		}
		
		result = true;
		tx_busy = 1;

		// Set length, start send which restarts tx interrupt
		CANCDMOB = (1 << CONMOB0) | message->length;
	}
	else if (TX_SIZE - ((tx_on - tx_off) & TX_ABS_MASK)) {
		// Copy data into TX buffer
		tx_frames[tx_on & (TX_SIZE-1)].id = message->id;
		tx_frames[tx_on & (TX_SIZE-1)].length = message->length;
		memcpy(tx_frames[tx_on & (TX_SIZE-1)].data, &message->data, CAN_FRAME_DATA_MAX_LENGTH);

		/*for (int i = 0; i < 8; i++) {
			tx_frames[tx_on & (TX_SIZE-1)].data[i] = message->data.u8[i];
		}*/

		tx_on++;
		result = true;
	}

	CANGIE |= (1 << ENIT);

	return result;
}