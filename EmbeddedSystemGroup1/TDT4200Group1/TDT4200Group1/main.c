/*
 * TDT4200Group1.c
 *
 * Created: 02.10.2017 13:17:16
 *  Author: hubi
 */ 

#define FOSC 4915200 // clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define set_bit(reg, bit) (reg |= (1<< bit))

#include "main.h"

void XMEM_Init() {
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
}

void init() {
	usart_init( MYUBRR );
	XMEM_Init();
	calibrateJoystick();
	DDRB &= ~0b0011;
	oled_init();
	oled_reset();
	fdevopen(oled_putchar, usart_getchar);
}

int main(void) {
	init();
	oled_reset();
	button_init();

	while(1) {
		
		test_oled();
	}
}