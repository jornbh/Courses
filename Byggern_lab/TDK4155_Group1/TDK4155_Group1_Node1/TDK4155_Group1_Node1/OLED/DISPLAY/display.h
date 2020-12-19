/*
 * animation.h
 *
 * Created: 17.11.2017 15:45:44
 *  Author: hubi
 */ 


#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../../main.h"

//MENU #################
void display_printCredits();
void display_printScores();
void display_printOptions();
void display_updateSlider(signed char slider);
void display_quit();

//GAME ######################
void display_gameStart();
void display_updateGame(int score, int mishap);
void display_showResults(int score, int result);

//ANIMATION #################
void display_startAnimation();



#endif /* ANIMATION_H_ */