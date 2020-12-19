/*
 * usart.h
 *
 * Created: 02.10.2017 13:27:34
 *  Author: hubi
 */ 


#ifndef USART_H_
#define USART_H_

#include "../main.h"

#define FOSC 4915200 // clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void usart_init(unsigned int ubrr);

int usart_getchar();

int usart_putchar(unsigned char data);

void printf_char(char* string) ;

void printf_int(int input);

void printf_hex(unsigned int input);

#endif /* USART_H_ */