/*
 * animation.c
 *
 * Created: 17.11.2017 15:45:14
 *  Author: hubi
 */ 

#include "display.h"
#include "../oled.h"

//GAME #############################################################

void display_gameStart() {
	oled_reset();
	oled_goto_pos(1, 5);
	printf("Start in:");
	
	oled_goto_pos(3, 25);
	printf("3");
	_delay_ms(1000);
	oled_goto_pos(4, 25);
	printf("2");
	_delay_ms(1000);
	oled_goto_pos(5, 25);
	printf("1");
	_delay_ms(1000);
	
	oled_reset(); 
}

void display_showBackHint() {
	oled_goto_pos(7, 17);
	printf("Left Button: Return");
}

void display_updateGame(int score, int mishap) {
	oled_goto_pos(1, 5);
	printf("Score: %d", score);
	oled_goto_pos(2, 5);
	printf("Mishap: %d/3", mishap);
	
	display_showBackHint();
}

void display_showResults(int score, int result) {
	oled_reset();
	oled_goto_pos(0, 19);
	printf("### GAME OVER! ###");
	oled_goto_pos(2, 5);
	printf("Your Score: %d", score);
	oled_goto_pos(4,  5); 
	if(result == 1) {
		printf("New Record!");
		oled_goto_pos(5, 5);
		printf("Congratulations!");
	} else if(result == 2) {
		printf("Second best Score!");
		oled_goto_pos(5, 5);
		printf("Super!");
	} else if(result == 3) {
		printf("Still in top 3!");
		oled_goto_pos(5, 5);
		printf("Not bad!");
	} else if(result == -1) {
		printf("No Score!");
		oled_goto_pos(5, 5);
		printf("Try again!");
	}
	
	display_showBackHint();
}

//MENU #############################################################

void display_printCredits() {
	oled_reset();
	oled_goto_pos(0, 5);
	printf("Credits:");
	oled_goto_pos(0, 88);
	printf("TDK4155");
	oled_goto_pos(2, 5);
	printf("GROUP 1:"),
	oled_goto_pos(3, 25);
	printf("Marte");
	oled_goto_pos(4, 25);
	printf("Jørn");
	oled_goto_pos(5, 25);
	printf("Hubert");
	
	display_showBackHint();
}

void display_printScores() {
	oled_reset();
	oled_goto_pos(0, 5);
	printf("Scores:");
	oled_goto_pos(2, 25);
	printf("1. %d", game_getHighScores(0));
	oled_goto_pos(3, 25);
	printf("2. %d", game_getHighScores(1));
	oled_goto_pos(4, 25);
	printf("3. %d", game_getHighScores(2));
	
	display_showBackHint();
}

void display_printOptions() {
	oled_reset();
	oled_goto_pos(0, 5);
	printf("Options:");
	oled_goto_pos(2, 5);
	printf("Speed-Tuning:");
	
	oled_goto_pos(5, 5);
	printf("right Button: confirm!");
	
	display_showBackHint();
}

void display_updateSlider(signed char slider) {
	int border = (slider + 100) / 2;
	printf_int(border);
	printf_char("\n");
	oled_goto_pos(3, 14);
	
	for(int i = 0; i < 100; i++)	{
		if(i < border) {
			oled_print_strip(0xFF);
			} else {
			oled_print_strip(0x00);
		}
	}
}

void display_quit() {
	oled_reset();

	for(int i = 0; i < 400; i++) {
		printf("Bye! Bye!");
		_delay_ms(50);
	}
}

//ANIMATION ########################################################

void oled_printZigZag(int line) {
	oled_goto_pos(line, 0);
	for(int column = 0; column < 8; column++) { //8 times each line
		
		for(int i = 0; i < 8; i++) {
			oled_print_strip((1 << i));
			_delay_ms(1);
		}
		for(int i = 8; i > 0; i--) {
			oled_print_strip((1 << i));
			_delay_ms(1);
		}
	}
}

void display_startAnimation() {
	oled_reset();
	oled_printZigZag(7);
	oled_printZigZag(0);
	oled_printZigZag(6);
	oled_printZigZag(1);
	oled_printZigZag(5);
	oled_printZigZag(2);
	
	oled_goto_pos(3, 33);
	for(int tmp = 0; tmp < 26; tmp++) {

		for(int i = 0; i < 18; i++) {
			oled_printchar('*');
		}
		
		oled_printchar(' ');
		oled_printchar('G');
		oled_printchar('R');
		oled_printchar('O');
		oled_printchar('U');
		oled_printchar('P');
		oled_printchar(' ');
		oled_printchar('1');
		oled_printchar(' ');
		
		_delay_ms(50);
	}
	
	oled_goto_pos(4, 17);
	for(int tmp = 0; tmp < 22; tmp++) {
		for(int i = 0; i < 13; i++) {
			oled_printchar('*');
		}
		oled_printchar(' ');
		oled_printchar('T');
		oled_printchar('D');
		oled_printchar('K');
		oled_printchar('4');
		oled_printchar('1');
		oled_printchar('5');
		oled_printchar('5');
		oled_printchar(' ');
		oled_printchar('*');
		oled_printchar('*');
		
		_delay_ms(50);
	}
	
	_delay_ms(1000);
}