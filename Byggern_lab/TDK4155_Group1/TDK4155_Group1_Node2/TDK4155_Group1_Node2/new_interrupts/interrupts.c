#include "./interrupts.h"
#include "./interrupts_usart.h"
#include "stdio.h"
#include "./interrupts_timer.h"
#include "../main.h"
void interrupts_test_interrupts(){

	interrupts_init();

	printf("Hello, interrupts\n");
}
void interrupts_init(){

	#ifdef INTERRUPTS
	
	interrupts_usart_init();

	interrupts_timer_init();

	sei();
	#else
	printf_char("Interrupts NOT enabled\n");
	#endif
}