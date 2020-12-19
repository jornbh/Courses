/*
 * game.c
 *
 * Created: 21.11.2017 12:55:31
 *  Author: huberts
 */ 

#include "game.h"
#include "../IR_D/irDiodes.h"
#include "../CAN/can.h"
#include "../new_interrupts/interrupts_timer.h"
#include "../PARSER/parser.h"

static char gameNumber = 0;
static boolean gameRunning = false; 

void game_handelPoints() {
	static int delay = 3000 / TIMER_PERIOD; // one point every second
	
	if(!gameRunning || ir_detectBeamBlocked()) { // Only counts if the game is running
		delay = 3000/TIMER_PERIOD;
		return; 
	}
	
	if(--delay == 0){
		delay = 3000/TIMER_PERIOD;
		printf("point\n");
		struct can_message msg = can_create_massage(ID_POINTS, 1, &gameNumber);
		can_send_message(&msg);
	}
}

void game_interrupt_handleIrBeam(){
	static int subCounter = 3; // Makes it so that a single low voltage can't be detected as a broken beam
	static int beamTimer = 50;

	if(ir_detectBeamBlocked()) {
		printf("*");
		if(beamTimer <= 0) {
			if (subCounter <= 0){ // Makes it so that a single low voltage can't be detected as a broken beam
				subCounter = 3;
				printf("\nBROKEN\n");
				game_sendBeamBrokenMessage();
				beamTimer = 50;
			} else {
				--subCounter;
			}
		} else {
			beamTimer = 50; // * 100ms
		}
	} else if( beamTimer > 0) {
		printf("-");
		--beamTimer;
		subCounter = 3;
		if( beamTimer == 0 ) {
			printf("\nREADY\n");
		}
	} else {
		subCounter = 3;
		printf("=");
	}
}


void game_initNewGame( char gameID){
	gameRunning = true; 
	gameNumber = gameID; 
	ir_enableBeam();
} 

void game_endGame(){
	ir_disableBeam(); 
	gameRunning = false; 
}

void game_sendBeamBrokenMessage(){
	//printf("SEND BROKEN MESSAGE %d, %d, %d", ID_GAME, 1, gameNumber); 
	struct can_message msg = can_create_massage(ID_GAME, 1, &gameNumber);
	can_send_message(&msg);
}
