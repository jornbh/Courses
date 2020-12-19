/*
 * solenoid.c
 *
 * Created: 13.11.2017 13:00:48
 *  Author: jornbh
 */ 

#include "solenoid.h"
#include "../new_interrupts/interrupts_timer.h"

static int solenoid_pushTimeRemaining = 0;

void solenoid_init() {
	//Set port as an output
	SET_BIT(DDRF, PF1);
		
	// set output as 0
	CLEAR_BIT(PORTF, PF1);
}

void solenoid_push() {
	SET_BIT(PORTF, PF1);
}

void solenoid_release() {
	CLEAR_BIT(PORTF, PF1);
}


void solenoid_interrupt_handlePush() {
	solenoid_pushTimeRemaining -= 1; 
	if(solenoid_pushTimeRemaining <= 0){
		solenoid_pushTimeRemaining = 20; 
		solenoid_release(); 
		//printf("Release push\n"); 
	}	
}

void solenoid_generatePush() {
	printf("Push\n"); 
	solenoid_pushTimeRemaining = 20;
	solenoid_push();
}