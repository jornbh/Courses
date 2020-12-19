/*
 * spi.h
 *
 * Created: 09.10.2017 10:21:39
 *  Author: martelm
 */ 

#include "../main.h"

#ifndef SPI_H_
#define SPI_H_

void spi_init();
void spi_startTransmision();
void spi_endTransmision();
void spi_transmitByte(char data);
char spi_readByte();


#endif /* SPI_H_ */