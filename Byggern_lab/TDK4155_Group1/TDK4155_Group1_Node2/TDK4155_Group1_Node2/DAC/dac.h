/*
 * dac.h
 *
 * Created: 06.11.2017 15:39:51
 *  Author: martelm
 */ 


#ifndef DAC_H_
#define DAC_H_



void dac_init();
void _mototInit();
void dac_setVoltage(unsigned char percentage);
void dac_conrolMotor(int joystickX);
// void regulator_PI(int refference_percent);

#define MOTOR_PORT PORTH

#define MOTOR_DIR_BIT PH1
#define MOTOR_SEL_BIT PH3
#define MOTOR_nRST_BIT PH6
#define MOTOR_nOE_BIT PH5
#define PLAYBOARD_ENCODER_RANGE 8400 // The total ammount of ticks to get from one side of the bord to the other

#define ENCODER_PIN PINK

#endif /* DAC_H_ */