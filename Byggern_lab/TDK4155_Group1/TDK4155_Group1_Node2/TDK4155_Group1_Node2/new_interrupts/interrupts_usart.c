/*
 * interrupts_timer.h
 *
 * Created: 14.11.2017 12:35:15
 *  Author: jornbh
 */ 

#include "./interrupts.h"
#include "./interrupts_usart.h"
#include "../USART/usart.h"
#include "../main.h"



// IF SOMEONE TRIES TO PRINT BEFORE sei() HAS BEEN USED, THE PROGRAM WILL CHRASH




void interrupts_usart_init(){
	usart_init(MYUBRR); // Must be run for the interrupt-based USART to work
	
	#ifdef USART_INTERRUPTS
		UCSR0B |= (1<< RXCIE0)|(1<< TXCIE0)|(1<< RXEN0)| (1<<TXEN0 ); // Enable Interrupts for transmiting and receiving , enable receiving and transmitting
		fdevopen(interrupt_usart_putchar, usart_getchar);
		sei();
		printf("Enabled usart-interrupts\n");
	#else
		printf_char("USART-interrupts NOT enabled\n"); // printf_char does not run on interrupts
	#endif
}


//USART-related interrupts
#ifdef USART_INTERRUPTS

void _writeToOutputRegister(char input);

// Global constants and variables

#define BUFFERLENGTH 50
static char buffer[BUFFERLENGTH];
static int buffer_readIndex =0;
static int buffer_writeIndex = 0; // The write-index can only overlap the read if the read is the one catching up to it

int interrupt_usart_putchar(char input, FILE* filler){

	if(buffer_writeIndex == buffer_readIndex){
		if(UCSR0A & (1<<UDRE0)){// Transmit-buffer is empty
			_writeToOutputRegister(input);
			return input;
		}
	}
	buffer_writeIndex = (buffer_writeIndex+1)%BUFFERLENGTH;
	while (buffer_readIndex == buffer_writeIndex){// TRIGGERS IF THE BUFFER IS FULL
		_delay_ms(1);
	}

	buffer[buffer_writeIndex] = input;
	return 0;
}


inline void _writeToOutputRegister(char input){
	
	UDR0 = input; //<– USART I/O Data Register n (<- WRITE HERE)

	// The transmit buffer can only be written when the UDREn Flag in the UCSRnA Register is set
}

//INTERRUPTS

ISR(USART0_TX_vect){
	// Transmit complete (USART 0)
	char old = buffer_readIndex;
	buffer_readIndex = (1+buffer_readIndex)%BUFFERLENGTH; // move to the next index
	if( buffer_readIndex == buffer_writeIndex){
		buffer_readIndex = old; // Prevents two successive characters in printf_char causing the register to be written again
		// Means the buffer is empty,
		// No more interrupts will be triggered if nothing is written to the register
		return;
	}

	_writeToOutputRegister( buffer[buffer_readIndex]);	
}

ISR(USART0_RX_vect) { //does nothing?

	// Receive complete (Not implemented (Not needed))
}


// ISR(USART1_TX_vect){

// 	// Transmit complete (USART 1) (Not implemented (Not needed))
// 	// There are two similair usarts on the arduino, we only need one of them
// 	// The IDE uses defines to separate between the different model-types, so it is possible to use
// 	//		#ifdef __<Some name>__ in orded to separate the ATmega 2560 and 162*
// 	//			*The 162 only has one USART
// }


#endif // USART_INTERRUPT