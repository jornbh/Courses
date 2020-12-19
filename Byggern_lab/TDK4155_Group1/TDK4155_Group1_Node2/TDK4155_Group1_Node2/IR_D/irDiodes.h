/*
 * irDiodes.h
 *
 * Created: 06.11.2017 08:51:01
 *  Author: martelm
 */ 

#include "../main.h"

#ifndef IRDIODES_H_
#define IRDIODES_H_

#define IR_COUNTER_BORDER 25

void ir_init();
void ir_enableBeam();
void ir_disableBeam();
void ir_handleIrBeam();
void ir_sendBeamBrokenMessage();
boolean ir_detectBeamBlocked();
void ir_interrupt_handleIrBeam(); 
int ir_getGameNumber(); 

#endif /* IRDIODES_H_ */