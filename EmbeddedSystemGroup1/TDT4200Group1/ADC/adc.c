/*
 * adc.c
 *
 * Created: 02.10.2017 13:22:53
 *  Author: hubi
 */ 

#include "adc.h"

int null_X, null_Y;

void calibrateJoystick(){
	null_X = null_Y =0;
	null_X = ADC_read(ADC_X_AXIS);
	null_Y = ADC_read(ADC_Y_AXIS);
}

uint8_t ADC_read( int channelNumber) {
	volatile uint8_t* ptr = (uint8_t*) 0x1400;
	*ptr = channelNumber;
	_delay_us(50);
	return *ptr;
}

struct Pos getJoystickPosition() {
	struct Pos position;
	
	position.posX = joyStickPositionToPercent(ADC_read(ADC_X_AXIS), true);
	position.posY = joyStickPositionToPercent(ADC_read(ADC_Y_AXIS), false);
	return position;
}

int joyStickPositionToPercent(int posit, boolean isX) {
	int nullPos;
	if(isX) {
		nullPos= null_X;
		} else {
		nullPos = null_Y;
	}
	
	int output = ((posit - nullPos)*100)/127;
	
	if(output > 100) {
		return 100;
		} else if(output < -100){
		return -100;
		} else {
		return output;
	}
}

int sliderPositionToPercent(int posit) {
	int nullPos = 128;
	int out = ((posit-nullPos)*100)/127;
	return out;
}

enum DIRECTION getJoystickDirection() {
	struct Pos p = getJoystickPosition();
	if(abs(p.posX) > abs(p.posY)) {
		if( p.posX > 10){
			return UP;
		}
		else if (p.posX < -10){
			return DOWN;
		}
		else {
			return NEUTRAL;
		}
	}
	else {
		if( p.posY > 10){
			return UP;
		}
		else if (p.posY < -10){
			return DOWN;
		}
		else {
			return NEUTRAL;
		}
	}
	return NEUTRAL;
}