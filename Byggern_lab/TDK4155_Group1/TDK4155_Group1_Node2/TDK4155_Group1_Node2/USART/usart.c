/*
 * USART.c
 *
 * Created: 23.10.2017 13:48:01
 *  Author: jornbh
 */ 

#include <avr/io.h>
#include "usart.h"

void usart_init( unsigned int ubrr){
	/* Set baud rate */
	UBRR0 = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	
	fdevopen(usart_putchar, usart_getchar);
} // USART_Init


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