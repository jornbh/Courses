/*
 * main.h
 *
 * Created: 02.10.2017 13:23:31
 *  Author: hubi
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 4915200


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>


#include "universal_header.h"
#include "ADC/adc.h"
#include "USART/usart.h"
#include "OLED/oled.h"
#include "OLED/MENU/menu.h"
#include "BUTTONS/buttons.h"
#include "CAN/can.h"
#include "MCP/mcp.h"
#include "GAME/game.h"


//ADDRESS-spaces
#define SRAM (char*)0x1800
#define ADC (char*)0x1400
#define OLED_C (char*)0x1000
#define OLED_D (char*)0x1200


#endif /* MAIN_H_ */