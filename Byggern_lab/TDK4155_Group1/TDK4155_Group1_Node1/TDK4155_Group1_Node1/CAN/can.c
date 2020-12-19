/*
 * can.c
 *
 * Created: 09.10.2017 14:07:58
 *  Author: martelm
 */ 

#include "can.h"
//#include "can_buffer.h"

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
		printf_char("MCP2515 error\n");
	}
	//else {
		//printf_char("can_init complete\n");
	//}
	
	can_setMode(CAN_MODE_NORMAL);
	
	//DEBUG ###########################
	//check Mode
	//printRegtister(MCP_CANCTRL, "Mode");
}

boolean can_error() {
	return false;
}

struct can_message can_create_massage(int id, char length, char data[]) {
	struct can_message msg;
	msg.id = id;
	msg.length = length;
	if (length > MAX_MESSAGE_LENGTH){
		printf_char("CAN length err\n");
		//printf_int(length);
		//printf_char(" Is larger than ");
		//printf_int((int)(MAX_MESSAGE_LENGTH));
		//printf_char("\n");
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
		printf_char("CAN all TX err\n");
		return;
	} else {
		printf_char("CAN no TX err\n");
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
	/* ???	
	char mask = 0x00;
	char setBits = 0x00;
	SET_BIT(mask, MCP_TXREQ);
	SET_BIT(setBits, MCP_TXREQ);
	mcp_bitModify(MCP_TXB0CTRL, mask, setBits);
	*/
		
	enum TRANSMIT_BUFFER trb = can_findFreeTransmitBuffer();
	int watchdogCounter = 0;
	while (trb == INVALID_TRANSMIT_BUFFER) {
		trb = can_findFreeTransmitBuffer();
		// Safety-features
		if(++watchdogCounter >= 0x7FFF){
			printf_char("CAN send err\n");
			_delay_ms(1000);
			return;
		}
		if (can_error()){
			//printf_char("The CAN-controller experienced an error, unable to continue program\n");
			return;
		}
	}
	
	can_message_to_buffer(msg, trb);
}

boolean can_transmit_complete() {
	return true;
}

int parseId(char* input){
	int output =  (input[1] >> 5) & 0x07;
	output += (((int)input[0] ) << 3);
	return output;
}

struct can_message can_recieve_message(enum RECIEVE_BUFFER rcb) {
	char idAdress, lengthAdress, dataAdress;

	mcp_bitModify(MCP_CANINTF, 0x01, 0x00);
	
	struct can_message recv_msg;
	
	if(rcb == RXB0) {
		idAdress = MCP_RXB0SIDH; 
		lengthAdress = MCP_RXB0DLC;
		dataAdress = MCP_RXB0D0;
	} else if(rcb == RXB1) {
		idAdress = MCP_RXB1SIDH;
		lengthAdress = MCP_RXB1DLC;
		dataAdress = MCP_RXB1D0;
	} else {
		printf_char("CAN recv err\n");
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
		//printf_char("No recieved massage!\n");
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

/* unused
enum RECIEVE_BUFFER can_findFullReceiveBuffer(char status){
	
	if((enum RECIEVE_BUFFER)status & RXB0) {
		return RXB0;
	} else if((enum RECIEVE_BUFFER)status & RXB1) {
		return RXB1;
	} else {
		printf_char("Error, no receive-buffer is full\n");
		return 0;
	}
};
*/

//is Ringbuffer used?
//void can_assignMessage(struct can_message* copy_onto, const  struct can_message* copy_from){
	//copy_onto->id = copy_from->id;
	//copy_onto->length = copy_from->length;
	//for(int i =0; i< copy_from->length; ++i){
		//copy_onto->data[i] = copy_from->data[i];
	//}
//}

/*
ISR(INT1_vect){
	
	// Interrupt when CAN receives a message
	printf_char( "Received message by interrupt\t");
	int temp = GIFR;
	printf_int(temp);
	printf_char("\n");
	//char status = mcp_readStatus();
	printf_char("CAN-status in interrupt: ");
	printf_int(temp);
	printf_char("\n");	
	enum RECIEVE_BUFFER buffer = can_findReceiveBuffer();
	
	// DEBUG
	buffer = can_findReceiveBuffer();
	if (buffer == INVALID_RECIEVE_BUFFER){
		printf_char("Invalid receive-buffer\t");
		printf_int(buffer);
		printf_char("\n");
	}
	
	// END DEBUG
	struct can_message recv = can_recieve_message(buffer);

	//Use the ring-buffer propperly
	#ifdef INCLUDE_CAN_BUFFER
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
*/


// ___________________DEBUG______________________________

void can_print_message(struct can_message* msg) {
	printf_char("CanM:\n ID: ");
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

void printRegtister(char reg, char* name) {
	char tmp;
	mcp_read(reg, &tmp, 1);
	printf_char(name);
	printf_char(": ");
	printf_int(tmp);
	printf_char("\n");
}

/* unused
// DEBUG-function
void waitUntilReadyToSend(){
	
	char temp;
	mcp_read(MCP_TXB0CTRL, &temp, 1);
	while( temp & 1<< MCP_TXREQ) {
		printf_char("\tError in transmition\r");
		mcp_read(MCP_TXB0CTRL, &temp, 1);
		printRegtister(MCP_EFLG, "MCP_EGLG");
		//if( temp & ( 1 << MCP_TXERR)){
		//mcp_write(MCP_TXB0CTRL, )
		//}
		printf_int(temp);
		temp =0;
		//mcp_write(MCP_TXB0CTRL,&temp, 1 );
		mcp_read(MCP_TXB0CTRL, &temp, 1);
		_delay_ms(1000);
	}
}
*/