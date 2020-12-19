/*
 * oled.h
 *
 * Created: 02.10.2017 13:29:08
 *  Author: hubi
 */ 


#include "../main.h"


#ifndef OLED_H_
#define OLED_H_


enum PRINTMODE{NORMAL, INVERSE};
	
void oled_init();

void oled_reset();

void oled_goto_line(char line);

void oled_goto_start_column();

void oled_goto_column(int column);

void oled_clear_line(char line);

void oled_goto_pos(char row, char column);

void oled_print_strip(char);

void oled_write_c(int input);

void oled_fill();

void oled_printchar(char input);

int oled_putchar(unsigned char input);

void oled_set_printmode(enum PRINTMODE);

void test_oled();

#endif /* OLED_H_ */