/*
 * Buttons.c
 *
 * Created: 02.10.2017 15:14:39
 *  Author: martelm
 */
 
#include "buttons.h"


void button_init() {
	PORTB = 0x01<<2;
	DDRB = 0x00;
}

boolean isJoystickButtonPressed() {
	short i = PINB;
	return !(0x1 & (i>>2));
}

boolean isLeftButtonPressed() {
	short i = PINB;
	return (0x1 & i);
}

boolean isRightButtonPressed() {
	short i = PINB;
	return (0x1 & (i>>1));
}