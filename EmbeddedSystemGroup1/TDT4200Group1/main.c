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
	calibrateJoystick();
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
	//init_interrupts();
	
	char* temp = "HELLO\n";
	int i=0;
	while(1) {
		//dummyPrint();
		//mcp_write(0x10, "HELLO\n",7);
		//printf_char(temp);
		//temp[0] = 'F';
		//mcp_read(0x10, temp,7);
		//printf_char("temp\n");
		//for (int i=0;i< 7; ++i){
			//printf_int(temp[i]);
		//}
		struct can_message msg;
		msg.id = 3;
		msg.length = 1;
		msg.data[0] = (uint8_t)'U'+ ++i;
		mcp_read(0x00, msg.data, msg.length);
		
		can_send_message(&msg);
		
	}
}