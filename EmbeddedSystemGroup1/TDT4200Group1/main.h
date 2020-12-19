/*
 * main.h
 *
 * Created: 02.10.2017 13:23:31
 *  Author: hubi
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 4915200

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

#include "ADC/adc.h"
#include "USART/usart.h"
#include "OLED/oled.h"
#include "BUTTONS/buttons.h"
#include "CAN/can.h"


//ADDRESS-spaces
#define SRAM (char*)0x1800
#define ADC (char*)0x1400
#define OLED_C (char*)0x1000
#define OLED_D (char*)0x1200


#endif /* MAIN_H_ */