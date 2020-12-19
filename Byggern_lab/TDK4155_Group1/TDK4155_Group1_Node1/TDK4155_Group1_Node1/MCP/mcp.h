/*
 * mpc.h
 *
 * Created: 09.10.2017 11:32:32
 *  Author: martelm
 */ 

#include "../universal_header.h"
#include  "../main.h"
#include "MCP2515.h"


#ifndef MCP_H_
#define MCP_H_

enum TRANSMIT_BUFFER{TXB0, TXB1, TXB2, TXBALL, INVALID_TRANSMIT_BUFFER = -1};
enum RECIEVE_BUFFER{RXB0 = 0b0001, RXB1 = 0b0010, INVALID_RECIEVE_BUFFER = -1};

void mcp_init();
void mcp_reset();
void mcp_enable_interrupt();
void mcp_bitModify(char adress, char mask, char data);
char mcp_readStatus();
void mcp_requestToSend(enum TRANSMIT_BUFFER txb);
void mcp_write(char adress, char* data, int length);
void mcp_read(char adress, char* outputArray, int length);


#endif /* MCP_H_ */