/*
 * fonts.c
 *
 * Created: 02.10.2017 13:31:09
 *  Author: hubi
 */ 

#include "fonts.h"

unsigned char fonts_getStrip(char character, char stripNo){
	character -= ' ';
	unsigned char temp = (pgm_read_word(&font5[character][stripNo]));
	return temp;
}