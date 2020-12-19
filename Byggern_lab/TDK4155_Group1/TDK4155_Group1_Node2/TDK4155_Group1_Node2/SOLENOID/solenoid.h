/*
 * solenoid.h
 *
 * Created: 13.11.2017 13:00:59
 *  Author: jornbh
 */ 


#ifndef SOLENOID_H_
#define SOLENOID_H_

#include "avr/io.h"
#include "../main.h"

void solenoid_init();
void solenoid_generatePush();
void solenoid_interrupt_handlePush(); 


#endif /* SOLENOID_H_ */