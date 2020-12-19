/*
 * interrupts_timer.h
 *
 * Created: 14.11.2017 12:35:15
 *  Author: jornbh
 */ 


#ifndef INTERRUPTS_TIMER_H_
#define INTERRUPTS_TIMER_H_

void interrupts_timer_init();

#define PRESCALER 256
#define PRESCALER_VALUE 3
 // p.157 in arduino datasheet (CSn2:0)
 // 1=> 1, 2=> 8, 3=> 64, 4=> 256, 5=> 1024
#define TIMER_PERIOD 50 // Given in milliseconds

//#define TIMER_UNIT 1000000
// #define CLOCK_TICKS TIMER_PERIOD*F_CPU/(PRESCALER_VALUE*TIMER_UNIT)  // Not used at the moment, but might be used later to get more than 1kHz interrupts 


#endif /* INTERRUPTS_TIMER_H_ */