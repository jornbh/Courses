/*
 * CFile1.c
 *
 * Created: 08.11.2017 17:22:48
 *  Author: jornbh
 */ 

#include "regulator.h"
#include "../USART/usart.h"
#include "../TIMER/timer.h"
#include "../DAC/dac.h"
static int reference = 0 ; 

static double K_p_PI = 3; 
static double K_i_PI = 0; 

void regulator_setReference(int newReference){
	reference = newReference; 
	//printf(" set refference to %d\n", reference); 
}

int _motorReadEncoder(); // Enables reading from the encoder 
float Kp, Ki;
//float reference;
int encoderOld, encoderDifference;
int integral;

void regulator_init(float P, float I) {
	Kp = P;
	Ki = I;
	timer_init();
}

void regulator_tuneSpeed(signed char slider) {
	char percentage = (slider + 100) / 2;
	Kp = (float) percentage / 100.0;
}

// HUBI's STUFF
// ____________________________________________
void regulator_updateEncoder(int enc) {
	encoderDifference = enc - encoderOld;
	encoderOld = enc;
}

void regulator_calculate() {
	
	if(reference == 0) {
		integral = 0;
	}
	
	int dt = timer_get(); //time since last call
	//printf("dt: %d\n", (int)dt );
	
	timer_reset();
	
	int currentSpeed = encoderDifference/dt; //can be negative
	//printf("currentSpeed: %d\n", (int)currentSpeed );
	
	int wantedSpeed = (reference*REGULATOR_MAX_SPEED)/100.0;
	
	int error = wantedSpeed - currentSpeed; 
	//printf("Error: %d\n", (int)error);
	
	integral += (error * dt)/100;
	//printf("Integral: %d\n", integral);
	
	float result = reference*Kp + Ki*integral;
	//printf("P: %d\n", (int) error);
	
	if(result > 100) {
		result = 100;
	} else if (result < -100) {
		result = -100;
	}
	
	//printf( "(wanted speed, currentSpeed): (%d, %d)\n ", (int)wantedSpeed, (int)currentSpeed); 
	dac_conrolMotor(result);
	
	regulator_updateEncoder(_motorReadEncoder());
}
// ____________________________________________

// Feed forward 
void regulator_feedForwardRegulator() {
	
	dac_conrolMotor(FEED_FORWARD_GAIN * reference);
}


void regulator_PI(){
		int position = _motorReadEncoder() * ENCODER_TICKS_SCALER; 
		static double I =0; 
		int dt = timer_get();
		
		I += dt* ((reference- position) * K_i_PI) ; 
		//printf("I %d\n", (int)(I*100)); 
		double u = K_p_PI*(reference - position) + I; 
		
		timer_reset(); 
		
		printf("u: %d I : %d\n", (int)(u), (int)(I*100) ); 
		dac_conrolMotor((int)u);
		
		//regulator_feedForwardRegulator(); 
		//static int I =0;
		//int motorPos = (_motorReadEncoder()*2 )/ 84;
		//int P = refference_percent - motorPos;
		//I += (motorPos - refference_percent);
		//printf( "u =  %d \n", P+ I );
		//dac_conrolMotor(0);

}