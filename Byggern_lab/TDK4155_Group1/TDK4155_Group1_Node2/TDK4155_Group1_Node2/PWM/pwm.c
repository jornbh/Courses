/*
 * pwm.c
 *
 * Created: 30.10.2017 14:56:22
 *  Author: martelm
 */ 


#include "pwm.h"
#include "avr/interrupt.h"
#include "../main.h"


#define F_Timer_1 16000000
#define TOP_TIMER_1 40000
#define PWM_CENTER 3000
//#define PWM_RANGE 1200 //alternating between 0.9-2.1ms
#define PWM_RANGE 1000 //just alternating between 1-2ms

void pwm_init(){
	// 1) use timer1 -> because this has 16Bit => n=1
	// 2) Set an output-pin as output for the PWM (Port B Pin 6)
	// 3) Set the prescaler and the TOP-value to be period of the PWM is 0.02s
	// 4) Enable Interrupts ????
	
	// 1)	
	// Set timer 1 to fast PWM-mode: WGMn3:0 = 0b1110 -> TOP = ICRn (with n = 1) ( better when TOP is rareely changed ) 
	CLEAR_BIT( TCCR1A, WGM10); // <- If set, OCRnA is used as TOP, if cleared ICRn is used instead
	SET_BIT( TCCR1A, WGM11);
	SET_BIT( TCCR1B, WGM12);
	SET_BIT( TCCR1B, WGM13);
	
	// 2)
	// Set OC1B (Port B pin 6) as the output of the PWM-signal in Non-inverted PWM-mode
	CLEAR_BIT( TCCR1A, COM1B0);
	SET_BIT( TCCR1A, COM1B1);
	SET_BIT(DDRB, 6); // Set the pin OC1B as an output

	// 3)
	// Set clock prescaler to 1/8 ( the clock-counter increments at 1/8 of the rate )
	CLEAR_BIT(TCCR1B, CS10);
	SET_BIT(TCCR1B, CS11);
	CLEAR_BIT(TCCR1B, CS12);

	// Set the value of TOP so that it is reached after 20ms (50Hz), clock-frequency = 16 MHz
	ICR1 =  TOP_TIMER_1;
	
	// 4) ?
	sei();
	
	//set PWm to default
	OCR1B = PWM_CENTER;
}

void pwm_controlDutyCycle(int joystickX) {
	
	// Prevents the values from becoming too large
	if (joystickX > 100){
		joystickX = 100;
	} else if(joystickX < -100){
		joystickX = -100;
	}
	
	double joystickX_percent = joystickX/100.0;
	
	//"Minus" because of direction
	double triggerPoint = PWM_CENTER - (joystickX_percent * PWM_RANGE);
	OCR1B = triggerPoint;
}

void pwm_setDefault() {
	OCR1B = PWM_CENTER;
}

//ISR(TIMER0_COMPA_vect) {
	//
//}