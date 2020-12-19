/*
 * menu.c
 *
 * Created: 02.10.2017 14:17:48
 *  Author: hubi
 */ 

#include "menu.h"


void menu_play(){

	const int menuLength = 2;
	const char* options[2] = {"en", "mer"};
	int pointer=menu_menuHandler(options, "Play", 2);

	}

void menu_options(){
	
	const char* options[2] = {"tull", "noe"};
	int pointer= menu_menuHandler(options, "Options", 2);
	
	
	}

void menu_mainMenu(){	
	void (*arr[3])(void)= { &menu_play, &menu_options, &menu_options};
	const char* options[3] = {"ONE   ", "TWO   ", "THREE   "};
	int pointer = menu_menuHandler(options, "Main Menu", 3);
	
	if(pointer >=0){
		arr[pointer]();
	}
	
	}

int menu_getNewPointerPosition(int position, int menuLength){
	static enum DIRECTION last = NEUTRAL;
	
	
	
	enum DIRECTION direction = getJoystickDirection();

	switch (direction){
		case UP:
		if( last == NEUTRAL){
			if( position> 0){
				--position;
			}
			else position = menuLength-1;
		}
		break;
		case DOWN:
		if( last == NEUTRAL){
			if ( position < menuLength-1){
				++ position;
			}
			else{
				position =0;
			}
		}
		break;
		case LEFT:
		break;
		case RIGHT:
		break;
		case NEUTRAL:
		break;
	}
	last = direction;
	return position;
}

void menu_printMenu(const char** options, int menuLength, int pointer, const char* title){
	oled_goto_pos(0,0);
	printf("       %s\n", title);
		for(int i = 0; i < menuLength; ++i){
			if(i== pointer){
				oled_set_printmode(INVERSE);
				printf("    ");
				
				printf(options[i]);
				oled_set_printmode(NORMAL);
			}
			else{
				printf("    ");
				printf(options[i]);
			}
			printf("\n");
		}
}

int menu_menuHandler(const char**options,const  char* title, int menuLength){
	oled_reset();
	menu_printMenu(options, menuLength, 0, title);
	while(buttons_isJoystickButtonPressed()); // Prevents double tapping on the joystick-button
	int pointer =0;
	while(1){
	menu_printMenu(options, menuLength, pointer, title);
	pointer = menu_getNewPointerPosition(pointer, menuLength);
	if (buttons_isJoystickButtonPressed()){
		return pointer;
	}
	
	
	
	
	if ( buttons_isJoystickButtonPressed()){
		return pointer;
	}
	if (buttons_isLeftButtonPressed()){
		return -1;
	}
	}
	
}


void menu_testMenu(){
	menu_mainMenu();
}
