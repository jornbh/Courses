/*
 * menu.c
 *
 * Created: 02.10.2017 14:17:48
 *  Author: hubi
 */ 

#include "menu.h"
#include "../../PARSER/parser.h"

void menu_scores() { //Or just show scores for a few secounds and go back?
	while(buttons_isJoystickButtonPressed()); // Prevents double tapping on the joystick-button
	display_printScores();
	while(!buttons_isLeftButtonPressed());
}

void menu_options() {

	display_printOptions();
	
	static char slider;
	
	while(!buttons_isLeftButtonPressed()) {
		slider = adc_getLeftSliderPosition();
		display_updateSlider(slider);
		if(buttons_isRightButtonPressed()) {
			oled_set_printmode(INVERSE);
			oled_goto_pos(5, 70);
			printf(" confirm ");
			struct can_message msg = can_create_massage(ID_OPTION, 1, &slider);
			can_send_message(&msg);	
			_delay_ms(100);
			oled_set_printmode(NORMAL);
			oled_goto_pos(5, 70);
			printf(" confirm ");
		}
		game_sendJoystickPosition();
	}	
}

void menu_credits() {
	while(buttons_isJoystickButtonPressed()); // Prevents double tapping on the joystick-button
	display_printCredits();
	while(!buttons_isLeftButtonPressed());
}

void menu_quit() {
	display_quit();
	//call cleanUp-Funktion? (is ther smth to clean up?
	while(1); //Not go back to the menu!
}

void menu_mainMenu() {	
	void (*mainArr[MAIN_MENU_SIZE])(void) = {&game_startGame, &menu_scores, &menu_options, &menu_credits, &menu_quit};
	const char* mainOptions[MAIN_MENU_SIZE] = {"New Game", "Scores", "Options", "Credits", "Quit"};

	int mainPointer = 0;
	
	while(1) {
		
		mainPointer = menu_menuHandler(mainOptions, "Main Menu", MAIN_MENU_SIZE);
		
		if(mainPointer >= 0){
			mainArr[mainPointer]();
			mainPointer = 0;
		}
	}	
}

int menu_getNewPointerPosition(int position, int menuLength){
	static enum DIRECTION last = NEUTRAL;
	
	enum DIRECTION direction = adc_getJoystickDirection();

	switch (direction) {
		case UP:
			if(last == NEUTRAL) {
				if( position> 0) {
					--position;
				} else position = menuLength-1;
			}
			break;
		case DOWN:
			if( last == NEUTRAL) {
				if (position < menuLength-1) {
					++ position;
				} else {
					position = 0;
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
	oled_goto_pos(0, 40);
	printf("%s", title);
	
	for(int i = 0; i < menuLength; ++i){
		oled_goto_pos(1+i, 20);
		if(i == pointer){
			oled_set_printmode(INVERSE);
			printf(" ");
			printf(options[i]);
			printf(" ");
			oled_set_printmode(NORMAL);
		} else {
			printf(options[i]);
			printf("   ");
		}
	}
}

int menu_menuHandler(const char** options, const char* title, int menuLength) {
	oled_reset();
	menu_printMenu(options, menuLength, 0, title);
	while(buttons_isJoystickButtonPressed()); // Prevents double tapping on the joystick-button
	int pointer = 0;
	while(1) {	
		pointer = menu_getNewPointerPosition(pointer, menuLength);	
		menu_printMenu(options, menuLength, pointer, title);
		
		if(buttons_isJoystickButtonPressed()) {
			printf_char("JS pressed\n");
			return pointer;
		} else if(buttons_isLeftButtonPressed()) {
			while(buttons_isLeftButtonPressed())
				printf_char("ret-1");
			return -1;
		}
	}
}