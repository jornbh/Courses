/*
 * mcp.c
 *
 * Created: 09.10.2017 11:32:21
 *  Author: martelm
 */ 
#include "../universal_header.h"
#include "mcp.h"
#include "../SPI/spi.h"
#include "avr/interrupt.h"

void mcp_init() {
	mcp_reset();
	//mcp_enable_interrupt();	
}

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
		//if(outputArray[i] != 0) {
			//printf_char("mcp_read: ");
			//printf_int(outputArray[i]);
			//printf_char("\n");
		//}
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

void mcp_requestToSend(enum TRANSMIT_BUFFER txb){
	spi_startTransmision();
	
	if(txb == TXB0) {
		spi_transmitByte(MCP_RTS_TX0);	
	} else if(txb == TXB1) {
		spi_transmitByte(MCP_RTS_TX1);	
	} else if(txb == TXB2) {
		spi_transmitByte(MCP_RTS_TX2);	
	} else if(txb == TXBALL) {
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
	
	//printf_char("read_Status: ");
	//printf_int(tmp);
	//printf_char("\n");
	
	return tmp;
}

void mcp_printStatus(char status) {
	status = status>>1; // We want to read register 1-3
	status &= 0x07; // Makes all other bits 0
	printf_int(status);
}

void mcp_bitModify(char adress, char mask, char data) {
	spi_startTransmision();
	spi_transmitByte(MCP_BITMOD);
	spi_transmitByte(adress);
	spi_transmitByte(mask);
	spi_transmitByte(data);
	spi_endTransmision();
}

void mcp_enable_interrupt() {
	//SET_BIT(DDRE, PE0); //SET PE0 input
	SET_BIT(MCUCR, ISC11);
	CLEAR_BIT(MCUCR, ISC10); 
	SET_BIT(GICR, INT1);

	mcp_bitModify(MCP_CANINTE, MCP_RX0IE | MCP_RX1IE, MCP_RX0IE | MCP_RX1IE); // Set the bits in MCU to get receive interrupts and transmit interrupts for buffer 0

	sei(); // Enable global interrupts by setting global interrupt enable bit in SREG
}


/*
ISR (BADISR_vect)    // Timer1 ISR 
{

	printf_char("UNIMPLEMENTED INTERRUPT\t");
	int temp = GIFR;
	printf_int(temp);
	printf_char("\n");
	sei();
}
*/