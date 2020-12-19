/*
 * mcp.c
 *
 * Created: 09.10.2017 11:32:21
 *  Author: martelm
 */ 

#include "mcp.h"
#include "../SPI/spi.h"

void mcp_reset() {
	spi_startTransmision();
	spi_transmitByte(MCP_RESET);
	spi_endTransmision();
}

void mcp_read(char address, char* outputArray, int length) {
	spi_startTransmision();
	
	spi_transmitByte(MCP_READ);
	spi_transmitByte(address);
	

	for(int i = 0; i < length; ++i){
		outputArray[i] = spi_readByte();
		if(outputArray[i] != 0) {
			printf_char("mcp_read: ");
			printf_int(outputArray[i]);
			printf_char("\n");
		}
	}
	
	spi_endTransmision();
}

void mcp_write(char adress, char* data, int length) {
	spi_startTransmision();
	
	spi_transmitByte(MCP_WRITE);
	spi_transmitByte(adress);
	
	for(int i = 0; i < length; ++i){
		spi_transmitByte(data[i]);
	}
	
	spi_endTransmision();
}

void mcp_requestToSend(int transmitBuffer) {
	spi_startTransmision();
	
	if(transmitBuffer == 0) {
		spi_transmitByte(MCP_RTS_TX0);	
	} else if(transmitBuffer == 1) {
		spi_transmitByte(MCP_RTS_TX1);	
	} else if(transmitBuffer == 2) {
		spi_transmitByte(MCP_RTS_TX2);	
	} else {
		spi_transmitByte(MCP_RTS_ALL);	
	}
	
	spi_endTransmision();
}

char mcp_readStatus() {
	//allows single instruction access to some of the often used status bits (for message reception and transmission)
	spi_startTransmision();
	
	spi_transmitByte(MCP_READ_STATUS);
	
	char tmp = spi_readByte();
	
	spi_endTransmision();
	
	return tmp;
}

void mcp_bitModify(char adress, char mask, char data) {
	spi_startTransmision();
	spi_transmitByte(MCP_BITMOD);
	spi_transmitByte(adress);
	spi_transmitByte(mask);
	spi_transmitByte(data);
	spi_endTransmision();
}