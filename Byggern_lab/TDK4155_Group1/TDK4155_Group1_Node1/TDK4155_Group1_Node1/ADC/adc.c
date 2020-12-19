/*
 * adc.c
 *
 * Created: 02.10.2017 13:22:53
 *  Author: hubi
 */ 

#include "adc.h"
#include "../universal_header.h"
int null_X, null_Y;

uint8_t adc_read( int channelNumber) {
	volatile uint8_t* ptr = (uint8_t*) 0x1400;
	*ptr = channelNumber;
	_delay_us(50);
	return *ptr;
}

void adc_calibrateJoystick(){
	null_X = null_Y = 0;
	null_X = adc_read(ADC_X_AXIS);
	null_Y = adc_read(ADC_Y_AXIS);
}

int adc_joyStickPositionToPercent(int posit, boolean isX) {
	int nullPos;
	if(isX) {
		nullPos = null_X;
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

struct Pos adc_getJoystickPosition() {
	struct Pos position;
	
	position.posX = adc_joyStickPositionToPercent(adc_read(ADC_X_AXIS), true);
	position.posY = adc_joyStickPositionToPercent(adc_read(ADC_Y_AXIS), false);
	return position;
}

int adc_getLeftSliderPosition() {
	return adc_sliderPositionToPercent(adc_read(ADC_L_SLIDER));
}

int adc_getRightSliderPosition() {
	return adc_sliderPositionToPercent(adc_read(ADC_R_SLIDER));
	//return adc_read(ADC_R_SLIDER);
}

int adc_sliderPositionToPercent(int posit) {
	int nullPos = 128;
	int out = ((posit-nullPos)*100)/127;
	return out;
}

enum DIRECTION adc_getJoystickDirection() {
	struct Pos p = adc_getJoystickPosition();
	if(abs(p.posX) > abs(p.posY)) {
		if( p.posX > 10){
			return UP;
		} else if (p.posX < -10){
			return DOWN;
		} else {
			return NEUTRAL;
		}
	} else {
		if( p.posY > 10){
			return UP;
		} else if (p.posY < -10){
			return DOWN;
		} else {
			return NEUTRAL;
		}
	}
	return NEUTRAL;
}

//DEBUG ################################################################

/*
void adc_printJoistickPosition(struct Pos joystick) {
	printf_char("Joystickposition: \n\tx: ");
	printf_int(joystick.posX);
	printf_char("\n\ty: ");
	printf_int(joystick.posY);
	printf_char("\n");
}
*/