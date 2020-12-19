/*
 * oled.c
 *
 * Created: 02.10.2017 13:28:24
 *  Author: hubi
 */ 

#include "oled.h"
#include "MENU/menu.h"


static int CURRENT_LINE = 0;
static int CURRENT_COL = 0;
static enum PRINTMODE printmode = NORMAL;

void oled_init() {
	oled_write_c(0xae);	//display off
	oled_write_c(0xa1);	//segment remap
	oled_write_c(0xda);	//common pads hardware: alternative
	oled_write_c(0x12);
	oled_write_c(0xc8);	//common output scan direction:com63~com0
	oled_write_c(0xa8);	//multiplex ration mode:63
	oled_write_c(0x3f);
	oled_write_c(0xd5);  //display divide ratio/osc. freq. mode
	oled_write_c(0x80);
	oled_write_c(0x81);  //contrast control
	oled_write_c(0x50);
	oled_write_c(0xd9);  //set pre-charge period
	oled_write_c(0x21);
	oled_write_c(0x20);  //Set Memory Addressing Mode
	oled_write_c(0x02);
	oled_write_c(0xdb);  //VCOM deselect level mode
	oled_write_c(0x30);
	oled_write_c(0xad);  //master configuration
	oled_write_c(0x00);
	oled_write_c(0xa4);  //out follows RAM content
	oled_write_c(0xa6);  //set normal display
	oled_write_c(0xaf);  //display on
}

void oled_write_c(int input) {
	volatile char* ptr = OLED_C;
	*ptr = (input);
}

void oled_reset() {
	for(int i = 0; i < 8; i++) {
		oled_clear_line(i);
	}
}

void oled_goto_line(char line) {
	if(line < 8) {
		oled_write_c(0xB0 + line);
		oled_write_c(0x00); //set lower column
		oled_write_c(0x10);
		CURRENT_LINE = line;
		CURRENT_COL = 0;
	}
}

void oled_goto_start_column() {
	oled_write_c(0x00); //set lower column
	oled_write_c(0x10);
	CURRENT_COL = 0;
}

void oled_goto_column(int column) {
	if(column < 128) {
		char lowerBitsOfColumn = column & 0x0F;
		oled_write_c(0x00 | lowerBitsOfColumn);
		char higherBitsOfColumn = (column>>4) & 0x0F;
		oled_write_c(0x10 | higherBitsOfColumn);
		CURRENT_COL = column;
	}
}

void oled_clear_line(char line) {
	oled_goto_line(line);
	for(int i = 0; i < 128; i++) {
		oled_print_strip(0x01);
	}
}

void oled_goto_pos(char line, char column) {
	oled_goto_line(line);
	oled_goto_column(column);
}

void oled_print_point(char row, char column) {
	oled_goto_pos(row/8 , column);
	int left_row = row%8;
	//1. read 1 Byte
	volatile char temp = 0; //read char isn´t working
	//2. go 1 Column back
	oled_goto_pos(row/8, column);
	//3. or it with row-bit
	temp |= 1<<left_row;
	//4. write it back
	oled_print_strip(temp);
}

void oled_print_strip(char input) {
	volatile char* ptr = OLED_D;
	*ptr = input;
}

void oled_printchar(char input) {
	char temp;
	char mask = 0x00;

	if (printmode == INVERSE){
		mask = 0xFF;
	}
	for(int i =0; i<5 ; ++i){
		temp = fonts_getStrip(input, i);
		oled_print_strip(temp^mask);
	}
}

int oled_putchar(unsigned char input) {
	if(input =='\n' || CURRENT_COL/5 >=25){
		CURRENT_LINE = ++CURRENT_LINE%8;
		for (int i = CURRENT_COL; i < 128; ++i){
			oled_print_strip(0x00);
		}
		oled_goto_line(CURRENT_LINE);
		usart_putchar('a');
		usart_putchar('0' + CURRENT_LINE);
		usart_putchar('\n');
		CURRENT_COL=0;
	}
	usart_putchar(input);
	if(input =='\n') return input;
	oled_printchar(input);

	CURRENT_COL+=5;
	return input;
}

void oled_set_printmode(enum PRINTMODE mode) {
	printmode = mode;
}

void test_oled() {
	menu_mainMenu();
}