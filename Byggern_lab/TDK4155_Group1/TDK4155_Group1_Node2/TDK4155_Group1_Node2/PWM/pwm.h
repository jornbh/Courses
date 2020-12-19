/*
 * pwm.h
 *
 * Created: 30.10.2017 14:56:04
 *  Author: martelm
 */ 


#ifndef PWM_H_
#define PWM_H_

void pwm_init();
void pwm_controlDutyCycle(int joystickX);
void pwm_setDefault();

#endif /* PWM_H_ */