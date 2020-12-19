/*
 * can_buffer.h
 *
 * Created: 23.10.2017 11:31:56
 *  Author: jornbh
 */ 


#ifndef CAN_BUFFER_H_
#define CAN_BUFFER_H_
#include "../universal_header.h"
void init_buffer();
boolean can_buffer_isNewMessage();
void can_buffer_addMessageToBuffer(const struct can_message* input);
struct can_message can_buffer_getMessage();


#endif /* CAN_BUFFER_H_ */