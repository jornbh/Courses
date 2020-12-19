/*
 * menu.h
 *
 * Created: 02.10.2017 14:18:03
 *  Author: hubi
 */ 

#include "../oled.h"
#include "../../ADC/adc.h"
#include "../../GAME/game.h"

#ifndef MENU_H_
#define MENU_H_

#define MAIN_MENU_SIZE 5
#define PLAY_MENU_SIZE 1
#define SCORE_MENU_SIZE 1
#define OPTION_MENU_SIZE 1
#define MUSIC_MENU_SIZE 0
#define CREDITS_MENU_SIZE 1


void menu_mainMenu();
int menu_getNewPointerPosition(int position, int menuLength);
void menu_printMenu(const char** options, int menuLength, int pointer, const char* title);
int menu_menuHandler(const char**options, const char* title, int menuLength);


#endif /* MENU_H_ */