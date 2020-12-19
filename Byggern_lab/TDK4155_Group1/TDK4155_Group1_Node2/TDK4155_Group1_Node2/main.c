/*
 * Arduino.c
 *
 * Created: 23.10.2017 10:07:28
 *  Author: jornbh
 */ 


#include <avr/io.h>

#include "main.h"
#include "USART/usart.h"
#include "CAN/can.h"
#include "PWM/pwm.h"
#include "IR_D/irDiodes.h"
#include "TWI/twi.h"
#include "avr/interrupt.h"
#include "DAC/dac.h"
#include "TIMER/timer.h"
#include "SOLENOID/solenoid.h"
#include "new_interrupts/interrupts.h"
#include "REGULATOR/regulator.h"
#include "SOUND/sound.h"


int getCounter();
void init(){
	usart_init ( MYUBRR );
	can_init();
	pwm_init();
	ir_init();
	dac_init(); // Interrupts must be enabled	
	interrupts_init();
	timer_init();
	solenoid_init();
	sound_init(); 
	sei(); 
}

int main(void) {

	init();
	printf("Hello World I`m Node2\n");
    while(1) {
		
		can_handleRecievedMessages();
		
		// a broken IR-beam is handled by interrupts 
		
		//ir_handleIrBeam();
		regulator_calculate();
		//printf( "counter %d\n",getCounter());
		//regulator_feedForwardRegulator();
		
    }
}