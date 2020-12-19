/*
 * can_buffer.c
 *
 * Created: 23.10.2017 10:08:59
 *  Author: jornbh
 */ 
#include "../universal_header.h"
#include "../main.h"
#include "can.h"

struct can_message* ringbuffer;
static int newestIndex;
static int lastReadMessageIndex;
static int bufferLength;


void init_buffer(){
	ringbuffer = malloc(10);
	newestIndex =0; 
	lastReadMessageIndex=0;
	bufferLength =10;
}

boolean can_buffer_isNewMessage(){
	if (lastReadMessageIndex == newestIndex){
		return false;
	}
	else
	{
		return true;
	}
}

void expandBuffer(){
	printf_char("Expanding Can-message-buffer\n");
	int newLength = 2*bufferLength;
	struct can_message* newBuffer = malloc(newLength);
	int newBufferIndex =0;
	for(int i =newestIndex; i< bufferLength; ++i, ++newBufferIndex){
		can_assignMessage(& (newBuffer[newBufferIndex]), &(ringbuffer[i]));

	}
	
	for(int i =0; i< newestIndex; ++i, ++newBufferIndex){
		can_assignMessage(& newBuffer[newBufferIndex], &ringbuffer[i]);
	}
	lastReadMessageIndex = 0; // So that the next read message is the first in the new buffer
	free(ringbuffer);
	ringbuffer = newBuffer;
	newestIndex = bufferLength;
	bufferLength = newLength;
}

void can_buffer_addMessageToBuffer(const struct can_message* input){
	newestIndex= (newestIndex+1)%bufferLength;
	if(newestIndex == lastReadMessageIndex){
		expandBuffer();
	}
	can_assignMessage(&ringbuffer[newestIndex], input);
	
	printf_char("Message added\n");
	
	
}

struct can_message can_buffer_getMessage(){
	printf_char("Getting message\t");
	printf_int(lastReadMessageIndex);
	printf_char(", ");
	printf_int(newestIndex);
	printf_char("\n");
	struct can_message msg;
	if (! can_buffer_isNewMessage()){
		printf_char("Error, no new message in buffer\n");
		return msg;
	}
	lastReadMessageIndex = (lastReadMessageIndex+1)%bufferLength;
	can_assignMessage(&msg, &ringbuffer[lastReadMessageIndex]);
	return msg;

}