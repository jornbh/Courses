#ifndef INRERRUPTS_H
#define INTERRUPTS_H

#define INTERRUPTS // COMMENT OUT TO REMOVE ALL INTERRUPTS





// If interrupt the specific types of interrupt are not defined, their are deactivated
#ifdef INTERRUPTS

#include "avr/interrupt.h"
#define TIMER_INTERRUPTS

//#define USART_INTERRUPTS
//#define CAN_INTERRUPTS // Does not seem to work properly



#endif // INTERRUPTS

void interrupts_test_interrupts();
void interrupts_init();




#endif