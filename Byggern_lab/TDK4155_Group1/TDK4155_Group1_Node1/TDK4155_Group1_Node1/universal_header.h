/*
 * universal_header.h
 *
 * Created: 25.10.2017 13:12:49
 *  Author: huberts
 */ 


#ifndef UNIVERSAL_HEADER_H_
#define UNIVERSAL_HEADER_H_

// Defined modes of operation
#define CAN_MODE_OF_OPERATION_NORMAL
//#define INTERRUPT

//Boolean
#define boolean int
#define true 1
#define false 0
#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))



#endif /* UNIVERSAL_HEADER_H_ */