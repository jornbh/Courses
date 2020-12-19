
#include "./interrupts.h"
#include "./interrupts_usart.h"
#include "stdio.h"
<<<<<<< HEAD
#include "avr/interrupt.h"
=======
#include "./interrupts_timer.c"
>>>>>>> b6bfe9ab2e3ddf532811abd046391254794345c5
void interrupts_test_interrupts(){

    interrupts_init();

    printf("Hello, interrupts");
}
void interrupts_init(){

    #ifdef INTERRUPTS
	
	
    interrupts_usart_init();
    interrupts_timer_init();

    sei();
    #endif
}

//ISR(BADISR_vect){
	//
	//printf("U\n");
//}