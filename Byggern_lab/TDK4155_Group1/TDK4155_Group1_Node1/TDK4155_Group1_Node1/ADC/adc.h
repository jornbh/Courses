/*
 * adc.h
 *
 * Created: 02.10.2017 13:21:57
 *  Author: hubi
 */ 

#ifndef ADC_H_
#define ADC_H_

#include "../main.h"
#include "../universal_header.h"

#define ADC_X_AXIS 5
#define ADC_Y_AXIS 4
#define ADC_L_SLIDER 6
#define ADC_R_SLIDER 7

struct Pos {
	int posX;
	int posY;
} pos;


enum DIRECTION{LEFT, RIGHT, UP, DOWN, NEUTRAL};

void adc_calibrateJoystick();

struct Pos adc_getJoystickPosition();
enum DIRECTION adc_getJoystickDirection();

int adc_getLeftSliderPosition();
int adc_getRightSliderPosition();
int adc_sliderPositionToPercent(int posit);


//DEBUG ##############################################
void adc_printJoistickPosition(struct Pos joystick);

#endif /* ADC_H_ */