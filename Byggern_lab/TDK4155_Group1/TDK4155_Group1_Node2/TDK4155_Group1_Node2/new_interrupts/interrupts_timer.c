/*
 * interrupts_timer.h
 *
 * Created: 14.11.2017 12:35:15
 *  Author: jornbh
 */ 

#include "avr/interrupt.h"
#include "avr/io.h"
#include "stdio.h"
#include "../main.h"
#include "./interrupts.h"
#include "./interrupts_timer.h"
#include "../IR_D/irDiodes.h"
#include "../GAME/game.h"
#include "../SOLENOID/solenoid.h"


#ifdef INTERRUPTS /* To avoid uneccecary headerfiles in the project */

#include "avr/interrupt.h"
#endif // INTERRUPTS



//#define TIMER_VALUE
void _setTimerInterruptMode();
void _setPrescaler();
void _setTimerValue(int milliseconds_timestep);
void _enableTimerInterrupts();


void interrupts_timer_init(){

	#ifdef TIMER_INTERRUPTS
	
	_setPrescaler();
	_setTimerValue(TIMER_PERIOD);
	_setTimerInterruptMode();
	_enableTimerInterrupts();
	// COMPLETE ???
	printf("Timer-interrupts enabled!\n");
	

	#else
	printf("Timer-interrupts are NOT enabled\n");
	#endif //TIMER_INTERRUPTS
}


#ifdef TIMER_INTERRUPTS
void _setTimerInterruptMode(){
	// Set to normal timer-mode instead of PWM-Mode
	// No timer-clear is done in normal-mode, it simply overruns.
	// Clear timer on Compare-match mode is acheived by setting the value of the bits to 4 (or 12)
	TCCR3A &= ~( ( 1 << WGM30)|( 1 << WGM31)); // 1 and 0 are supposed to be cleared in this register,  the others are unknown
	CLEAR_BIT(TCCR3B, WGM33);
	SET_BIT(TCCR3B, WGM32);
	printf("TCCR3A %d \n ", TCCR3A);
	// The COM 0 and 1-bits must also be set
	// set to 0b11 to set OCnx on compare-match ( Prefered )
	//  to 0 for to disconnect OCnx
	TCCR3A |= (1<< COM3A0) | (1 << COM3A1);
	printf("Timer interrupt mode set,TCCR3A = %d \n", TCCR3A);

}

void _setPrescaler(){
	// Make sure the prescaler is set to a value fitting all devices using a clock
	int temp = PRESCALER_VALUE;
	TCCR3B |= ( 0b0111 & temp ); // p.157 in arduino datasheet (CSn2:0)
	//PRESCALER_VALUE =  1=> 1, 2=> 8, 3=> 64, 4=> 256, 5=> 1024

	return;
	// If the OCIE-bit is set, an interrupt happens on match
	printf("Prescaler set TCCR3B = %d\n", TCCR3B);

}


void _setTimerValue(int milliseconds_timestep){


	//________________________________________________________________________
	// The counter is constantly compared with OCRnx (16- bit for some timers, 8 for others)
	// 16: 1(A,B,C) , 3(A,B,C) 4(A,B,C) , 5(A,B,C)
	// 8: THE REST (2 ? )

	long temp= (long)((milliseconds_timestep * F_CPU )/ ((long)1000*PRESCALER));
	//OCR3A = CLOCK_TICKS;
	OCR3A = (int)temp;
	//OCRnxH = (char)(0xFF&temp); // Both H and L- registers can usually be set by OCRnx= temp
	//OCRnxL = (char)(0xFF &(temp>>8));
	printf("Timer value set OCR3A = %d, (milliseconds = %lu )\n", OCR3A, temp );
	//________________________________________________________
}



void _enableTimerInterrupts(){

	SET_BIT(TIMSK3, ICIE3);
	// • Bit 5 – ICIEn: Timer/Countern, Input Capture Interrupt Enable
	// When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally enabled), the
	// Timer/Countern Input Capture interrupt is enabled

	SET_BIT(TIMSK3, OCIE3A); // X = A,B,C ( pin [3:1])
	// Enables interrupts on compare-match on compare-match A,B or C
	SET_BIT(TIMSK3, TOIE3); // Bit 0
	// Clearing the bit disables interrupts when the timer overflows
	
	TCNT3 =0; // Ensures the timer does not have to loop around an entire time for the first interrupt
	printf("timer-interrupts enabled\n");
}


#endif





// JUST DEMO_-functions to show the interrupts work
//________________________________________________
static int counter=0, overflow =0;
int getCounter(){
	return counter;
}
int getOverflow(){
	return overflow;
}
boolean last = true;
//___________________________________________________


 

ISR(TIMER3_COMPA_vect, ISR_NOBLOCK){ // It seems like the interrupts might mess with _delay(), causing an infinite loop in some cases

	game_interrupt_handleIrBeam();
	game_handelPoints(); 
	solenoid_interrupt_handlePush();

	++counter;  // Just example-code to show the interrupts are able to count

}


ISR(TIMER3_OVF_vect, ISR_NOBLOCK){
	//printf("*"); // To show an error has occured
	++overflow;
}