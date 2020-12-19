/*
 * game.c
 *
 * Created: 30.10.2017 13:57:36
 *  Author: martelm
 */ 

#include "game.h"

boolean rightButtonIsReady = true;
struct Pos oldPos;
int oldRightSliderPosition;
boolean gameRunning = false;
boolean irBeamEnabled = false;

static char gameNumber = 0;
static int mishap = 0;
static int score = 0;
int highScores[3] = {0, 0, 0};


void game_sendJoystickPosition() {
	struct Pos joystick = adc_getJoystickPosition();
	
	if(joystick.posX != oldPos.posX || joystick.posY != oldPos.posY) {
		char position[2];
		position[0] = joystick.posX;
		position[1] = joystick.posY;

		struct can_message msg = can_create_massage(ID_JOYSTICK_POSITION, 2, position);
		can_send_message(&msg);
	}
	oldPos = joystick;
}

void game_handleRightButton() {
	boolean isPressed = buttons_isRightButtonPressed();
	//boolean isPressed = buttons_isJoystickButtonPressed();
	
	if(isPressed && rightButtonIsReady) {
		char pressed = isPressed;
		struct can_message msg = can_create_massage(ID_RIGHT_BUTTON, 1, &pressed);
		can_send_message(&msg);
		//printf_char("send Right Button pressed!\n");
		rightButtonIsReady = false;
	} else if(!isPressed) {
		rightButtonIsReady = true;	
	}
}

void game_sendRightSliderPosition() {
	int rightSliderPosition = adc_getRightSliderPosition();
	
	//printf_char("rightSlider: ");
	//printf_int(rightSliderPosition);
	//printf_char("\n");
	
	if(abs(rightSliderPosition - oldRightSliderPosition) > 2) {
		char rsp = rightSliderPosition;
		struct can_message msg = can_create_massage(ID_RIGHT_SLIDER, 1, &rsp);
		can_send_message(&msg);
	}
	
	oldRightSliderPosition = rightSliderPosition;
}

void game_init() {
	gameRunning = true;
	mishap = 0;
	score = 0;
	//enable IR-BEAM
	gameNumber++;
	if(gameNumber == 0) {
		gameNumber++;
	}
	
	display_gameStart();
	
	struct can_message msg = can_create_massage(ID_GAME, 1, &gameNumber);
	can_send_message(&msg);
}

void game_quit() {
	gameRunning = false;
	
	//disable IR-BEAM
	char irBeamEnabled = false;
	struct can_message msg = can_create_massage(ID_GAME, 1, &irBeamEnabled);
	can_send_message(&msg);
	
	//store score
	if(highScores[0] < score) {
		highScores[2] = highScores[1];
		highScores[1] = highScores[0];
		highScores[0] = score;
		display_showResults(score, SCORE_FIRST);
		} else if(highScores[1] < score) {
		highScores[2] = highScores[1];
		highScores[1] = score;
		display_showResults(score, SCORE_SECOND);
		} else if(highScores[2] < score) {
		highScores[2] = score;
		display_showResults(score, SCORE_THIRD);
		} else {
		display_showResults(score, SCORE_LOSE);
	}
}

void game_handleMishaps(char gameID) {
	
	if(gameID == gameNumber) { //ignoring old mishaps
		++mishap;
		
		if(mishap >= 3) {
			game_quit();
			
			while(!buttons_isLeftButtonPressed());
		}	
	}
}

void game_startGame() {
	
	game_init();
	
	while(gameRunning) {
		display_updateGame(score, mishap);
		can_handleRecievedMessages(); //-> can quit the game
		game_sendJoystickPosition();
		game_sendRightSliderPosition();
		game_handleRightButton();	
		if( buttons_isLeftButtonPressed()){
			gameRunning = false;
			break;
		}
	} 
}

void game_incrementScore(){
	++score; 
}

int game_getHighScores(int number) {
	return highScores[number];	
}