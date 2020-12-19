/*
 * can.h
 *
 * Created: 09.10.2017 14:08:20
 *  Author: martelm
 */ 

#include "..\main.h"
#include "..\MCP\mcp.h"


#ifndef CAN_H_
#define CAN_H_

#define CAN_REGISTER_ADDRESS_MODESELECT 0xF0
#define CAN_MODE_LOOPBACK 0x40
#define CAN_MODE_OPERATIONAL 0x00
#define CAN_MASK_MODE 0xE0

struct can_message{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
	};

void can_init();
void can_setMode(char mode);
void can_send_message(struct can_message* msg);

#endif /* CAN_H_ */