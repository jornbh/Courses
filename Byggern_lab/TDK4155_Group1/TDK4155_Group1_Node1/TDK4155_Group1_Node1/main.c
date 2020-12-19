/*
 * TDT4200Group1.c
 *
 * Created: 02.10.2017 13:17:16
 *  Author: hubi
 */ 

#define FOSC 4915200 // clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include "main.h"


void XMEM_Init() {
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
}

void init() {
	usart_init( MYUBRR );
	XMEM_Init();
	adc_calibrateJoystick();
	DDRB &= ~0b0011;
	oled_init();
	oled_reset();
	fdevopen(oled_putchar, usart_getchar);
	oled_reset();
	buttons_init();
	can_init();
}

int main(void) {

	init();
	printf_char("Hello World I`m Node1\n");
	display_startAnimation();
	
	menu_mainMenu();
	
	while(1) {
		printf_char("F\n");
	}
}