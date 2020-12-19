
#ifndef INRERRUPTS_H
#define INTERRUPTS_H

#define INTERRUPTS // COMMENT OUT TO REMOVE ALL INTERRUPTS

//#define INTERRUPTS 




// If interrupt is not defined, no aditional libraries are included
// and none of the sub-interrupts are defined or implemented
#ifdef INTERRUPTS
#define TIMER_INTERRUPTS

#define USART_INTERRUPTS


#define USART_INTERRUPT
#include "avr/interrupt.h"

#endif // INTERRUPTS 

void interrupts_test_interrupts();
void interrupts_init();




#endif