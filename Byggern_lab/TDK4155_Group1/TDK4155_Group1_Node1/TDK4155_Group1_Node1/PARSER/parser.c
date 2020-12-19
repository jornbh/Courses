/*
 * parser.c
 *
 * Created: 30.10.2017 14:20:21
 *  Author: martelm
 */ 

#include "parser.h"
#include "../GAME/game.h"

void parser_handleRecievedMessages(struct can_message* msg) {
	int id = msg->id;
	
	switch(id){
		case(ID_GAME):
			//can_print_message(msg);
			game_handleMishaps(msg->data[0]);
			break;
		case(ID_POINTS):
			game_incrementScore();
			break;
		default:
			printf_char("msgID err\n");
			break;
	}
}