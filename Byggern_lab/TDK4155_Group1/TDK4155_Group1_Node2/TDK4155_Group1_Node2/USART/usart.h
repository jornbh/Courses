/*
 * usart.h
 *
 * Created: 23.10.2017 13:48:14
 *  Author: jornbh
 */ 


#ifndef USART_H_
#define USART_H_

#include "../main.h"

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


void usart_init(unsigned int ubrr);

int usart_getchar();

int usart_putchar(unsigned char data);

void printf_char(char* string) ;

void printf_int(int input);

void printf_hex(unsigned int input);

void init_interrupts();
void dummyPrint();

#endif /* USART_H_ */