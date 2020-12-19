/*
 * usart.c
 *
 * Created: 02.10.2017 13:26:57
 *  Author: hubi
 */ 

#include "usart.h"

void usart_init( unsigned int ubrr ) {
	// set the baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	// enable reciever and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

	
	//fdevopen(USART_putchar, USART_getchar);
}

int usart_getchar() {
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

int usart_putchar(unsigned char data) {
	// wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0)));
	// put data into buffer, sends the data
	UDR0 = data;
	return data;
}

void printf_char(char* string) {
	for (int i = 0; string[i] != '\0'; ++i) {
		usart_putchar(string[i]);
	}
}

void printf_int(int input) {
	if (input<0){
		input *=-1;
		usart_putchar('-');
	}
	if(input/10 > 0)
	printf_int(input/10);
	usart_putchar((unsigned char) (input%10) + '0');
}

void printf_hex(unsigned int input){
	int temp;
	for (int i=0; i<4; ++i){
		temp = (input& 0xF000)>>12;
		input= input<<4;
		if ( temp> 9){
			usart_putchar( 'A' + temp -10);
		}
		else{
			usart_putchar( '0' + temp);
		}
	}
}


//#include <avr/interrupt.h>
//static int c =0;
//static char lastChar =-1;
//void dummyPrint(){
	//if( c== lastChar) return;
	//
	//usart_putchar(c+'a');
	//lastChar=c;
//}
//void init_interrupts(){
//
		//TCNT1 = 0xFFFF;   // for 1 sec at 16 MHz
//
		//TCCR1A = 0x00;
		//TCCR1B = (1<<CS12);;  // Timer mode with 1024 prescler
		//TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
		//sei(); // Enable global interrupts by setting global interrupt enable bit in SREG
	//
//}
//
//ISR (TIMER1_OVF_vect)    // Timer1 ISR
//{
	//++c;
//}