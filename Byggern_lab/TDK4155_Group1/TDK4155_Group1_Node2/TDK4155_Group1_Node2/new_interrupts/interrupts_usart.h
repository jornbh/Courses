/*
 * interrupts_timer.h
 *
 * Created: 14.11.2017 12:35:15
 *  Author: jornbh
 */ 

#ifndef INTERRUPTS_USART_H
#define INTERRUPTS_USART_H

#include "interrupts.h"
#include "avr/io.h"
#include "stdio.h"
void interrupts_usart_init();
int interrupt_usart_putchar(char input, FILE* filler);



#endif//INTERRUPTS_USART_H