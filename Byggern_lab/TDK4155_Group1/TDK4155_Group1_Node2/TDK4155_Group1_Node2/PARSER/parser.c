/*
 * parser.c
 *
 * Created: 30.10.2017 14:20:21
 *  Author: martelm
 */ 

#include "parser.h"
#include "../PWM/pwm.h"
#include "../DAC/dac.h"
#include "../SOLENOID/solenoid.h"
#include "../REGULATOR/regulator.h"
#include "../IR_D/irDiodes.h"
#include "../GAME/game.h"
int makeSignedInt(char input);
void parser_printJoystickPosition(char data[]);

void parser_handleRecievedMessages(struct can_message* msg) {
	int id = msg->id;
	switch(id){	
		case(ID_JOYSTICK_POSITION):
			regulator_setReference(makeSignedInt(msg->data[0]));
			break;
		case(ID_GAME):
			if(msg->data[0] != 0) {
				game_initNewGame(msg->data[0]);
			} else {
				game_endGame(); 
				regulator_setReference(0);
				pwm_setDefault();
			}
			break;
		case(ID_RIGHT_BUTTON):
			//call function which generates the pulse
			printf("right Button pushed!\n");
			solenoid_generatePush();
			break;
		case(ID_RIGHT_SLIDER):
			//printf("SLIDER\n");
			printf("RightSlider: %d\n", makeSignedInt(msg->data[0]));
			pwm_controlDutyCycle(makeSignedInt(msg->data[0]));
			break;
		case(ID_OPTIONS):
			regulator_tuneSpeed(msg->data[0]);
			break;
		default:
			printf("Unknown message-ID\n");
			break;
	}
}


// Makes an 8-bit char into a signed int
int makeSignedInt(char input){
	if (input & 0x80){
		return -255+ (int)input;
	}
	else{
		return (int)input;
	}
}

//DEBUG #############################################################

void parser_printJoystickPosition(char data[]) {
//	printf("Joystick Position:\n\t x: %d\n\t y: %d\n", data[0], data[1]);
	printf_char("Joystick-Position: \n\tx: ");
	printf_int(makeSignedInt(data[0]));
	printf_char("\n\ty: ");
	printf_int(makeSignedInt(data[1]));
	printf_char("\n");
}