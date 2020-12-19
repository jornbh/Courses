/*
 * adc.h
 *
 * Created: 02.10.2017 13:21:57
 *  Author: hubi
 */ 

#ifndef ADC_H_
#define ADC_H_

#include "../main.h"

#define ADC_X_AXIS 5
#define ADC_Y_AXIS 4
#define ADC_L_SLIDER 6
#define ADC_R_SLIDER 7

struct Pos {
	int posX;
	int posY;
} pos;


enum DIRECTION{LEFT, RIGHT, UP, DOWN, NEUTRAL};
	
uint8_t ADC_read( int channelNumber);

struct Pos getJoystickPosition();

int joyStickPositionToPercent(int posit, boolean isX);

int sliderPositionToPercent(int posit);

void calibrateJoystick();

enum DIRECTION getJoystickDirection();

#endif /* ADC_H_ */