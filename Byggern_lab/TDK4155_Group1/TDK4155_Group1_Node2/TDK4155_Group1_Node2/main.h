/*
 * main.h
 *
 * Created: 23.10.2017 13:58:50
 *  Author: jornbh
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000

//Boolean
#define boolean int
#define true 1
#define false 0
#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>


#include "USART/usart.h"


#endif /* MAIN_H_ */