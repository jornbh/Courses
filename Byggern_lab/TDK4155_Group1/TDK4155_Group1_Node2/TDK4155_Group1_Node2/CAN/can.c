/*
 * can.c
 *
 * Created: 09.10.2017 14:07:58
 *  Author: martelm
 */ 

#include "can.h"
#include "../MCP/mcp.h"
#include "../SPI/spi.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include "can_buffer.h"
#include "../PARSER/parser.h"

void can_setMode(char mode) {
	mcp_bitModify(MCP_CANCTRL, CAN_MASK_MODE, mode);
}

void can_init() {

	spi_init();
	mcp_init();
	
	//self-test
	char value;
	mcp_read(MCP_CANSTAT, &value, 1);
	if((value & MODE_MASK) != MODE_CONFIG){
		printf("MCP2515 is NOT in configuration mode after reset! MCP_CANSTAT =%d\n", value);
	} else {
		printf_char("can_init complete\n");
	}
	
	can_setMode(CAN_MODE_NORMAL);
	
	//check mode
	char mode;
	mcp_read(MCP_CANCTRL, &mode, 1);
	//printf_char("mode: ");
	//printf_int(mode);
	//printf_char("\n");
	printf("Can-Mode: %d <- should be 7!\n", mode);
}

struct can_message can_create_massage(int id, char length, char data[]) {
	struct can_message msg;
	msg.id = id;
	msg.length = length;
	if (length > MAX_MESSAGE_LENGTH){
		printf_char("Unable to make a message of the desired length\n");
		printf_int(length);
		printf_char(" Is larger than ");
		printf_int((int)(MAX_MESSAGE_LENGTH));
		printf_char("\n");
		msg.length = MAX_MESSAGE_LENGTH;
	}
	for(int i = 0; i < msg.length; i++)	{
		msg.data[i] = data[i];
	}
	return msg;
}

void can_message_to_buffer(struct can_message* msg, enum TRANSMIT_BUFFER txb) {
	char idAdress, lengthAdress, dataAdress;
	
	if(txb == TXB0) {
		idAdress = MCP_TXB0SIDH;
		lengthAdress = MCP_TXB0DLC;
		dataAdress = MCP_TXB0D0;
	} else if(txb == TXB1) {
		idAdress = MCP_TXB1SIDH;
		lengthAdress = MCP_TXB1DLC;
		dataAdress = MCP_TXB1D0;
	} else if(txb == TXB2) {
		idAdress = MCP_TXB2SIDH;
		lengthAdress = MCP_TXB2DLC;
		dataAdress = MCP_TXB2D0;
	} else if(txb == TXBALL) {
		printf_char("you can only fill one buffer at a time!\n");
		return;
	} else {
		printf_char("invalid transmit_buffer\n");
		return;
	}
	
	// SET THE ID in the MCP
	char ID[2];
	ID[0] = (msg->id >> 3) & 0xFF; // High bits of the ID
	ID[1] = (msg->id & 0x07) << 5; // Low bits of the ID
	mcp_write(idAdress, ID, 2);
	
	// Set the length of the CAN-message 
	char length = msg->length;
	mcp_write(lengthAdress, &length, 1);
	// Fill in the Data of the CAN-message
	if(length != 0) {
		mcp_write(dataAdress, &msg->data[0], length);	
	}
	
	//send it
	mcp_requestToSend(txb);
}

enum TRANSMIT_BUFFER can_findFreeTransmitBuffer() {
	char status;
	
	mcp_read(MCP_TXB0CTRL, &status, 1);
	if(! (status & (1 << 3) ) ) {
		return TXB0;
	}
	
	mcp_read(MCP_TXB1CTRL, &status, 1);
	if(! (status & (1 << 3) ) ) {
		return TXB1;
	}
	
	mcp_read(MCP_TXB2CTRL, &status, 1);
	if(! (status & (1 << 3) ) ) {
		return TXB2;
	}
	
	return INVALID_TRANSMIT_BUFFER;
}

void can_send_message(struct can_message* msg) {
	//find free transieve_buffer
	
	enum TRANSMIT_BUFFER trb = can_findFreeTransmitBuffer();
	int watchdogCounter = 0;
	while (trb == INVALID_TRANSMIT_BUFFER) {
		trb = can_findFreeTransmitBuffer();
		// Safety-features
		if(++watchdogCounter >= 0x7FFF){
			printf("CAN-Message aborted due to timeout when attempting to find transmit-buffer\n"); 
			_delay_ms(1000);
			return;
		}
		if (can_error()){
			printf_char(" The CAN-controller experienced an error, unable to continue program\n");
			return;
		}
	}
	
	//printf("Message to Buffer!\n");
	can_message_to_buffer(msg, trb);
}

boolean can_error() {
	return false;
}

int parseId (char* input){
	int output =  (input[1] >> 5) & 0x07;
	output += (((int)input[0] ) << 3);
	return output;
}

struct can_message can_recieve_message(enum RECIEVE_BUFFER rcb) { //TODO: check id
	
	char idAdress, lengthAdress, dataAdress;
	struct can_message recv_msg;
	
	mcp_bitModify(MCP_CANINTF, 0x01, 0x00);
	
	if(rcb == RXB0) {
		idAdress = MCP_RXB0SIDH; 
		lengthAdress = MCP_RXB0DLC;
		dataAdress = MCP_RXB0D0;
	} else if(rcb == RXB1) {
		idAdress = MCP_RXB1SIDH;
		lengthAdress = MCP_RXB1DLC;
		dataAdress = MCP_RXB1D0;
	} else {
		printf_char("invalid recieve_buffer!\n");
		recv_msg = can_create_massage(-1,-1,0);
		return recv_msg;
	}

	char tmp[2];
	mcp_read(idAdress, &tmp[0], 2);
	int id = parseId(tmp);
	char length;
	mcp_read(lengthAdress, &length, 1);
	char data[MAX_MESSAGE_LENGTH];
	mcp_read(dataAdress, &data[0], length);
	
	recv_msg = can_create_massage(id, length, data);

	return recv_msg;
}

void can_int_vect() {
	
}

enum RECIEVE_BUFFER can_findFullReceiveBuffer(char status) {
	
	if((enum RECIEVE_BUFFER)status & RXB0) {
		return RXB0;
	} else if((enum RECIEVE_BUFFER)status & RXB1) {
		return RXB1;
	} else {
		printf_char("Error, no receive-buffer is full\n");
		return 0;
	}
};

enum RECIEVE_BUFFER can_findReceiveBuffer() {
	char canInterrupt;
	mcp_read(0x2C, &canInterrupt, 1);
	
	if(canInterrupt & 0b01) {
		return RXB0;
	}
	else if(canInterrupt & 0b10) {
		return RXB1;
	}
	else {
		//printf_char("Error, no receive-buffer is full\n");
		return INVALID_RECIEVE_BUFFER;
	}
}

void can_handleRecievedMessages() {
	enum RECIEVE_BUFFER buffer = can_findReceiveBuffer();
	if(buffer != INVALID_RECIEVE_BUFFER) {
		struct can_message recv = can_recieve_message(buffer);
		//can_print_message(&recv);
		parser_handleRecievedMessages(&recv);
	}
}

//used for buffer
void can_assignMessage(struct can_message* copy_onto, const  struct can_message* copy_from){
	copy_onto->id	  = copy_from->id;
	copy_onto->length = copy_from->length;
	for(int i = 0; i< copy_from->length; ++i) {
		copy_onto->data[i] = copy_from->data[i];
	}
}

//Interrupt-handler
ISR(INT1_vect){
	
	// Interrupt when CAN receives a message
	printf_char("Received message by interrupt\n");
	char status = mcp_readStatus();
	enum RECIEVE_BUFFER buffer = can_findFullReceiveBuffer(status);
	struct can_message recv = can_recieve_message(buffer);
	
	#ifdef INCLUDE_CAN_BUFFER
	
	//Use the ring-buffer propperly
	
	printf( "Adding message to buffer..\n");
	//can_buffer_addMessageToBuffer(&recv);
	#endif //INCLUDE_CAN_BUFFER

	#ifndef INCLUDE_CAN_BUFFER
	printf_char("Printing\n");
	can_print_message(&recv);
	#endif //INCLUDE_CAN_BUFFER
	printf_char("Inerrupt done\n");
	sei();
}


//DEBUG-FUNCTION################################################################################


void can_read_transmit_buffer() { //TODO: check id
	char idAdress, lengthAdress, dataAdress;
	
	mcp_bitModify(MCP_CANINTF, 0x01, 0x00);

	idAdress = MCP_TXB0SIDH;
	lengthAdress = MCP_TXB0DLC;
	dataAdress = MCP_TXB0D0;
	

	char tmp[2];
	mcp_read(idAdress, &tmp[0], 2);
	int id = parseId(tmp);
	char length;
	mcp_read(lengthAdress, &length, 1);
	char data[MAX_MESSAGE_LENGTH];
	mcp_read(dataAdress, &data[0], length);
	
	struct can_message recv_msg = can_create_massage(id, length, data);
	
	printf("transmit_buffer:\n");
	can_print_message(&recv_msg);
}

void can_print_message(struct can_message* msg) {
	printf_char("Can_Message: \n	ID: ");
	printf_int(msg->id);
	printf_char("\n	length: ");
	printf_int(msg->length);
	printf_char("\n	data: ");
	for(int i = 0; i < msg->length; i++) {
		printf_int(msg->data[i]);
		printf_char(",");
	}
	printf_char("\n");
}