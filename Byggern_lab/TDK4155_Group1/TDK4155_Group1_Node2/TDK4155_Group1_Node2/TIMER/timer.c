/*
 * timer.c
 *
 * Created: 08.11.2017 18:15:01
 *  Author: jornbh
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

int volatile static timer = 0;

void timer_init() {
	//set prescaler to 1024
	//TCCR5B |= (1 << CS50) | (1 << CS52);
	
	TCCR5B |= (1 << CS50); //no prescaling -> why so slow?
	//Overflow interrupt enable
	TIMSK5 |= (1 << TOIE5);
}

void timer_stop() {
	TCCR5B &= ~((1 << CS50) | (1 << CS52));
	TIMSK5 &= ~(1 << TOIE5);
}

int timer_get() {
	return timer;
}

void timer_reset() {
	timer = 0;
}

ISR(TIMER5_OVF_vect) {
	timer++;
}