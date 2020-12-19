/*
 * can.c
 *
 * Created: 09.10.2017 14:07:58
 *  Author: martelm
 */ 

#include "can.h"
#include "../MCP/mcp.h"
#include "../SPI/spi.h"

void can_init() {

	spi_init();
	mcp_reset();
		
	//self-test
	char value;
	mcp_read(MCP_CANSTAT, &value, 1);
	if((value & MODE_MASK) != MODE_CONFIG){
		printf_char("MCP2515 is NOT in configuration mode after reset!\n");
	}
	
	//loop back mode
	can_setMode(CAN_MODE_LOOPBACK);
	
	char status = mcp_readStatus();
	printf_int(status);
	printf_char("\n");

}

void can_setMode(char mode) {
	 mcp_bitModify(CAN_REGISTER_ADDRESS_MODESELECT, CAN_MASK_MODE, mode);
}

void can_send_message(struct can_message* msg) {
	int structLength = msg->length + 2;
	char buffer[structLength];
	buffer[0] = msg->id;
	buffer[1] = msg->length;
	for(int i = 0; i < msg->length; i++) {
		buffer[i+2] = msg->data[i];	
	}
	mcp_write(0x00, &buffer[0], structLength);
}

void can_error() {
	
}

boolean can_transmit_complete() {
	return true;
}

void can_data_recieve() {
	
}

void can_int_vect() {
	
}

