/*
 * sound.c
 *
 * Created: 20.11.2017 19:13:30
 *  Author: martelm
 */ 

#include "sound.h"
#include "../main.h"
#include "../new_interrupts/interrupts_timer.h"





void sound_init(){
	SET_BIT(DDRK, PK7); // PORK K pin 7 is A15 on the shield and is used to activate an external module for sound
	sound_turnOn(false); 
}


void sound_turnOn(boolean turnOn){
	if(turnOn){
		SET_BIT(PORTK, PK7); // PK7 activates an external module for sound
	}
	else{
		CLEAR_BIT(PORTK, PK7);
	}
	
}


