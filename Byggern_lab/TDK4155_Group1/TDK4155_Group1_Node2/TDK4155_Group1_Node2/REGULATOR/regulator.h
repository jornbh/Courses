/*
 * regulator.h
 *
 * Created: 08.11.2017 17:23:17
 *  Author: jornbh
 */ 


#ifndef REGULATOR_H_
#define REGULATOR_H_


//#define FEED_FORWARD

#define REGULATOR_MAX_SPEED 50
#define FEED_FORWARD_GAIN 0.8
#define ENCODER_TICKS_SCALER 200.0/(8700) // Converts form encoder-ticks to percent ( (2)* total board-length / ( 100%))

void regulator_init(float P, float I);
void regulator_tuneSpeed(signed char slider);
void regulator_update(float ref);
void regulator_updateEncoder(int enc);
void regulator_calculate();


void regulator_setReference(int newReference); 


/// use regulator_setReference() to set the reference the regulators regulate towards
void regulator_feedForwardRegulator(); 





#endif /* REGULATOR_H_ */