/*
 * latch.c
 *
 * Created: 11.09.2017 08:57:01
 *  Author: hubi
 */ 

#include <avr/io.h>

#define F_CPU 4915200 

#include <util/delay.h>

#define FOSC 4915200 // clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr ) {	
	// set the baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	// enable reciever and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

unsigned char USART_recieveData() {
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_transmitData(unsigned char data) {
	// wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0)));
	// put data into buffer, sends the data
	UDR0 = data;
}

void printf(char* string) {
	for (int i = 0; string[i] != '\0'; ++i){
		USART_transmitData(string[i]);
	}
}

void printf_int(int input){
	if(input/10 > 0)
		printf_int(input/10);
	USART_transmitData((unsigned char) (input%10) + '0');
}

void XMEM_Init(){
	MCUCR |= 0x80; //Setting the SRE-Bit!? 
}
void init(){
		USART_Init ( MYUBRR );
		XMEM_Init();
}
int main(void) {
	init();
	volatile int* ptr= (int*) 0xFF;
	DDRE = 0xFF;
	PORTE = 2;
    while(1) {
		*ptr = 0xFF;
	}
}