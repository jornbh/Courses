/*
 * game.h
 *
 * Created: 21.11.2017 12:55:43
 *  Author: huberts
 */ 


#ifndef GAME_H_
#define GAME_H_

void game_interrupt_handleIrBeam();
void game_handelPoints();
void game_initNewGame( char gameID); 
void game_endGame(); 
void game_sendBeamBrokenMessage(); 



#endif /* GAME_H_ */