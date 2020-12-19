/*
 * spi.c
 *
 * Created: 09.10.2017 10:21:21
 *  Author: martelm
 */ 

#include "spi.h"

void spi_init() {
	/* Set MOSI and SCK and SS output, all others input */
	DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB7); // pin 1,2 and 7 are clock, MOSI and chip-select. pin 0 goes nowhere, but the arduino belives it is the slave-select, so it must be active (low). 
	CLEAR_BIT(PORTB, PB0);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void spi_startTransmision() {
	//put SS-Signal to low
	CLEAR_BIT(PORTB, PB7);
}

void spi_endTransmision() {
	//put SS-Signal to high
	SET_BIT(PORTB, PB7);
}

void spi_transmitByte(char byte) {
	
	SPDR = byte;
	while( ! (SPSR & (1<<SPIF))) {
	}
}

char spi_readByte() {
	
	spi_transmitByte(0x00);
	char tmp = SPDR;
	
	return tmp;
}

void spi_readInterrupt() {
	char interupt = PE0;
	printf_char(&interupt);
}