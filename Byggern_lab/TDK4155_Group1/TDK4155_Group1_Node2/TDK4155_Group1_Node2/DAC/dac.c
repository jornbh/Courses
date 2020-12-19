/*
 * dac.c
 *
 * Created: 06.11.2017 15:40:04
 *  Author: martelm
 */ 
#include "../main.h"
#include "dac.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "../TWI/twi.h"
#include "stdio.h"
#include "../REGULATOR/regulator.h"

void dac_init() {
	sei();
	TWI_Master_Initialise();
	_mototInit();
	regulator_init(0.4, 0.2);	
}

void dac_setVoltage(unsigned char percentage) {
	unsigned char message[3];
	message[0] = 0x5E; // Addressing the DAC
	message[1] = 3; // Addressing DAC0?
	message[2] = (0xFF * percentage) / 100; // Setting the value to a percentage of the max-voltage
	
	TWI_Start_Transceiver_With_Data(message, 3);
}

void _setMotorDirectionForward(boolean forward){
	if(forward) {
		SET_BIT(MOTOR_PORT, MOTOR_DIR_BIT);
	} else {
		CLEAR_BIT(MOTOR_PORT, MOTOR_DIR_BIT);
	}
}

void _mototInit() {
	// Eable interraction with motorbox
	SET_BIT(DDRH, MOTOR_DIR_BIT); // Set the dir-pin as output
	
	// Set the SEL-pin
	SET_BIT(DDRH, MOTOR_SEL_BIT);
	
	//reset Encoder
	DDRH |= (1<< MOTOR_nRST_BIT) | (1<< MOTOR_nOE_BIT);
	CLEAR_BIT(MOTOR_PORT, MOTOR_nRST_BIT);
	_delay_ms(20);
	SET_BIT(MOTOR_PORT, MOTOR_nRST_BIT);
	
	dac_setVoltage(0);
}

int _motorReadEncoder() {
	MOTOR_PORT &= ~((1<<MOTOR_SEL_BIT) | (1<< MOTOR_nOE_BIT));
	_delay_us(20);
	char msb = ENCODER_PIN;
	int encoder = (msb << 8);
	SET_BIT(MOTOR_PORT, MOTOR_SEL_BIT);
	_delay_us(20);
	char lsb = ENCODER_PIN;
	encoder |= lsb;

	return -encoder; // In order for right to be the positive direction
}


void dac_conrolMotor(int power) {
	
	if(power >= 0) {
		//set direction pin +
		_setMotorDirectionForward(true);
		dac_setVoltage(power);
	} else {
		//set direction pin -?
		_setMotorDirectionForward(false);
		dac_setVoltage(-power);
	}
}
