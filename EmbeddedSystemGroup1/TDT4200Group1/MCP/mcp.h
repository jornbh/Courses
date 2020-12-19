/*
 * mpc.h
 *
 * Created: 09.10.2017 11:32:32
 *  Author: martelm
 */ 


#include  "../main.h"
#include "MCP2515.h"


#ifndef MCP_H_
#define MCP_H_

void mcp_bitModify(char adress, char mask, char data);
char mcp_readStatus();
void mcp_requestToSend(int transmitBuffer);
void mcp_write(char adress, char* data, int length);
void mcp_read(char adress, char* outputArray, int length);
void mcp_reset();





#endif /* MCP_H_ */