/*
 * irDiodes.c
 *
 * Created: 06.11.2017 08:50:42
 *  Author: martelm
 */ 

#include "irDiodes.h"
#include "avr/io.h"
#include  "../CAN/can.h"
#include "../PARSER/parser.h"



boolean irBeamEnabled = false;
boolean irBeamReady = false;

void ir_init() {
	//Enable ADC
	SET_BIT(ADCSRA, ADEN);
	
	//set multiplexers, to enable one analog input (at spezific port)
	// --> writing MUX bits in ADMUX and ADCSRB
	//Set Bit3 of ADCSRB to 0
	CLEAR_BIT(ADCSRB, MUX5);
	//set Bits4:0 in ADMUX to 0 -> to select ADC0 (PORT F) as input
	CLEAR_BIT(ADMUX, MUX0);
	CLEAR_BIT(ADMUX, MUX1);
	CLEAR_BIT(ADMUX, MUX2);
	CLEAR_BIT(ADMUX, MUX3);
	CLEAR_BIT(ADMUX, MUX4);

	//set Port as an input
	CLEAR_BIT(DDRF, PF0);
	
	//start 
	SET_BIT(ADCSRA, ADSC); //single conversion
	//SET_BIT(ADCSRA, ADATE);
}

void ir_enableBeam() {
	printf("IR-enabled\n"); 
	irBeamEnabled = true;
}

void ir_disableBeam() {
	irBeamEnabled = false;
	printf("IR-Beam disabled!\n");
}

boolean ir_detectBeamBlocked() {
	
	while(ADCSRA & (1<< ADSC)){
		//printf("waiting...\n");
	}
	
	char irL = ADCL;
	char irH = ADCH; //read both, to finish the conversion
	
	//printf("IRL: %d\n", irL);
	
	//set single conversation again
	SET_BIT(ADCSRA, ADSC); 
	//printf("irL: %d \t irH: %d \t", irL, irH);
	if(irL < 10 || irH != 0) {
		return true;

	}
	return false;
}

//void ir_handleIrBeam() {
	//if(irBeamEnabled) {
		//boolean current = ir_detectBeamBlocked();
		//if(current) {
			//irCounter++;
			//if(irBeamReady && irCounter > IR_COUNTER_BORDER) {
				//printf("IR BEAM BLOCKED! #######################\n");
				//
				////create and send can massage
				//struct can_message msg = can_create_massage(ID_GAME, 1, &gameNumber);
				//can_send_message(&msg);
				//irBeamReady = false;
			//}
		//} else {
			//irCounter = 0;
			//irBeamReady = true;
		//}	
	//}
//}




