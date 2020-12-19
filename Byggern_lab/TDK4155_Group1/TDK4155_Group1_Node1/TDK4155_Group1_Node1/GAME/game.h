/*
 * game.h
 *
 * Created: 30.10.2017 13:57:47
 *  Author: martelm
 */ 


#ifndef GAME_H_
#define GAME_H_

#include "../ADC/adc.h"
#include "../CAN/can.h"
#include "../BUTTONS/buttons.h"
#include "../PARSER/parser.h"
#include "../OLED/DISPLAY/display.h"

#define SCORE_FIRST 1
#define SCORE_SECOND 2
#define SCORE_THIRD 3
#define SCORE_LOSE -1

void game_sendJoystickPosition();
void game_handleRightButton();
void game_sendRightSliderPosition();

void game_startGame();
void game_handleMishaps(char gameID);
int game_getHighScores(int number);
void game_incrementScore(); 


#endif /* GAME_H_ */