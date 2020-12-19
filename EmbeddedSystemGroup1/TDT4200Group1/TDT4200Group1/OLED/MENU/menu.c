/*
 * menu.c
 *
 * Created: 02.10.2017 14:17:48
 *  Author: hubi
 */ 

#include "menu.h"


void menu_play(){
	oled_reset();
	while(! isLeftButtonPressed()){
	oled_goto_pos(0,0);
	const int menuLength = 2;
	char* options[2] = {"en", "mer"};
	static int pointer=0;
	printf("Play\n");
	menu_printMenu(options, menuLength,pointer);
	pointer = menu_getNewPointerPosition(pointer, menuLength);
}
	}

void menu_options(){
	oled_reset();
	while(! isLeftButtonPressed()){
	oled_goto_pos(0,0);
	const int menuLength = 2;
	char* options[2] = {"tull", "noe"};
	static int pointer=0;
	printf("Options\n");
	menu_printMenu(options, menuLength,pointer);
	pointer = menu_getNewPointerPosition(pointer, menuLength);
	}
	}

void menu_mainMenu(){
	//PRINT THE MENU
	oled_reset();
	while(! isLeftButtonPressed()){
	oled_goto_pos(0,0);
	printf("         MAIN MENU          \n");
	void (*arr[3])(void)= { &menu_play, &menu_options, &menu_options};
	const int menuLength = 3;
	char* options[3] = {"ONE   ", "TWO   ", "THREE   "};
	static int pointer = 0;
	
	menu_printMenu(options, menuLength, pointer);
	
	pointer = menu_getNewPointerPosition(pointer, menuLength);
	
	if (isJoystickButtonPressed()){
		arr[pointer]();
	}
	
	
	
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
void menu_printMenu(char** options, int menuLength, int pointer){
	
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

