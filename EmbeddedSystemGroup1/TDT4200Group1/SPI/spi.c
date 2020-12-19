/*
 * spi.c
 *
 * Created: 09.10.2017 10:21:21
 *  Author: martelm
 */ 

#include "spi.h"

void spi_init() {
	
	/* Set MOSI and SCK and SS output, all others input */
	DDRB = (1<<PB5)|(1<<PB7)|(1<<PB4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void spi_startTransmision() {
	//put SS-Signal to low
	CLEAR_BIT(PORTB, PB4);
}

void spi_endTransmision() {
	//put SS-Signal to high
	SET_BIT(PORTB, PB4);
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

