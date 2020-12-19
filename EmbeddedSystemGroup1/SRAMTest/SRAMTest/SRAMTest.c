/*
 * SRAMTest.c
 *
 * Created: 11.09.2017 11:10:24
 *  Author: hubi
 */ 

#include <avr/io.h>
#include <stdlib.h>

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

void printf_int(int input) {
	if(input/10 > 0)
		printf_int(input/10);
	USART_putchar((unsigned char) (input%10) + '0');
}

void XMEM_Init() {
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);
}

void init() {
	USART_Init( MYUBRR );
	XMEM_Init();
}

void SRAM_test(void) {
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf_char("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			//printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			/*
			printf_char("Write phase error: ext_ram[");
			printf_int(i);
			printf_char("] = ");
			printf_int(retreived_value);
			printf_char(" (should be ");
			printf_int(some_value);
			printf_char(")\n");
			*/
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed); // reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			//printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			
			/*printf_char("Retrieval phase error: ext_ram[");
			printf_int(i);
			printf_char("] = ");
			printf_int(retreived_value);
			printf_char(" (should be ");
			printf_int(some_value);
			printf_char(")\n");
			*/
			retrieval_errors++;
		}
	}
	//printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
	printf_char("SRAM test completed with \n");
	printf_int(write_errors);
	printf_char(" errors in write phase and \n");
	printf_int(retrieval_errors);
	printf_char(" errors in retrieval phase\n\n");
}

int main(void) {
	init();
	SRAM_test();
	
	//int* ptr_oled_cs = (int*) 0x17FF;
	//ptr_oled_cs[0] = 10;
	while(1) {
		//int* ptr_oled_cs = (int*) 0x11FF;
		//*ptr_oled_cs = 10;
		//_delay_ms(1000);
		//int* ptr_oled_dc = (int*) 0x1200;
		//*ptr_oled_dc = 10;
		//_delay_ms(1000);
		int* ptr_adc = (int*) 0x1400;
		*ptr_adc = 10;
		//_delay_ms(1000);
		//int* ptr_sram = (int*) 0x1800;
		//*ptr_sram = 10;
		//_delay_ms(1000);
	}
}