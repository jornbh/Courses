/*
 * menu.h
 *
 * Created: 02.10.2017 14:18:03
 *  Author: hubi
 */ 

#include "../oled.h"

#ifndef MENU_H_
#define MENU_H_


void menu_mainMenu();
int menu_getNewPointerPosition(int position, int menuLength);
void menu_printMenu(char** options, int menuLength, int pointer);
#endif /* MENU_H_ */