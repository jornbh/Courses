/*
 * parser.h
 *
 * Created: 30.10.2017 14:20:31
 *  Author: martelm
 */ 


#ifndef PARSER_H_
#define PARSER_H_

#include "../CAN/can.h"

//Massage-ID`s
#define ID_JOYSTICK_POSITION 1
#define ID_GAME 2
#define ID_RIGHT_BUTTON 3
#define ID_RIGHT_SLIDER 4
#define ID_OPTION 5
#define ID_POINTS 6

void parser_handleRecievedMessages(struct can_message* msg);

#endif /* PARSER_H_ */