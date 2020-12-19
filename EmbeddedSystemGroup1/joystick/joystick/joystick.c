/*
 * joystick.c
 *
 * Created: 18.09.2017 10:38:45
 *  Author: hubi
 */ 

#include <avr/io.h>
#include <stdlib.h>

#define F_CPU 4915200 

#include <util/delay.h>

#define FOSC 4915200 // clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define ADC_X_AXIS 5
#define ADC_Y_AXIS 4
#define ADC_L_SLIDER 6
#define ADC_R_SLIDER 7

enum Direction {LEFT, RIGHT, UP, DOWN, NEUTRAL};

struct Pos {
	int posX;
	int posY;
} pos;

void USART_Init( unsigned int ubrr ) {
	// set the baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	// enable reciever and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

void XMEM_Init() {
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
}

void init() {
	USART_Init( MYUBRR );
	XMEM_Init();
	DDRB &= 0; 
}

unsigned char USART_getchar() {
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_putchar(unsigned char data) {
	// wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0)));
	// put data into buffer, sends the data
	UDR0 = data;
}


void printf_char(char* string) {
	for (int i = 0; string[i] != '\0'; ++i) {
		USART_putchar(string[i]);
	}
}

void printf_int(unsigned int input) {
	if(input/10 > 0)
		printf_int(input/10);
	USART_putchar((unsigned char) (input%10) + '0');
}

void printf_hex(unsigned int input){
	int temp;
	for (int i=0; i<4; ++i){
		temp = (input& 0xF000)>>12;
		input= input<<4;
		if ( temp> 9){
			USART_putchar( 'A' + temp -10);
		}
		else{
			USART_putchar( '0' + temp);
		}
	}
}

uint8_t ADC_read( int channelNumber) {
	volatile uint8_t* ptr = (uint8_t*) 0x1400;
	*ptr = channelNumber;
	_delay_us(50);
	return *ptr;
}

struct Pos getJoystickPosition() {
	struct Pos position;
	
	
	position.posX = 0;
	position.posY = 0;
	return position;
}

enum Direction getJoystickDirection() {
	if(0 == 0) {
		return DOWN;
	} else if (0 == 0) {
		return LEFT;
	} //...
	return NEUTRAL;
}

int main(void) {
	init();
	
	volatile uint8_t x,y;
	volatile uint8_t l,r;
	volatile uint8_t buttons;
	
	while(1) {
		
		y = ADC_read(ADC_Y_AXIS);
		x = ADC_read(ADC_X_AXIS);
		printf_char("x: ");
		printf_int(x);
		printf_char("\t y: ");
		printf_int(y);
		
		l = ADC_read(ADC_L_SLIDER);
		r = ADC_read(ADC_R_SLIDER);
		printf_char("\t\t ls: ");
		printf_int(l);
		printf_char("\t rs: ");
		printf_int(r);
		
		buttons = PORTB;
		printf_char("\t\t buttons: ");
		printf_int(buttons);
		printf_char("\n");
	}
}