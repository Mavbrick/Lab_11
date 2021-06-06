/*	Author: Maverick Bautista
 *      Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 11  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "scheduler.h"
#include "timer.h"
#include "bit.h"
#include "keypad.h"
#endif

/* implement the SMs */
unsigned char reset = 0x00;
//unsigned char start = 0x00;
unsigned char p1column = 0x80; //column 8 for paddle
unsigned char p1row = 0xF1; //paddle is 3 led's wide

enum Player1States { p1Static, p1Up, p1Down};
int Player1Fct(int state) {
	static unsigned char x;
        x = GetKeypadKey();
	reset = ~PINB & 0x04;
	switch(state) {
		case p1Static:
			if(reset == 0x04) {
				state = p1Static;
				p1row = 0xF1;
			}
			else if(x == '\0') {
				state = p1Static;
			}
			else if(x == '1') {
				state = p1Up;
			}
			else if(x == '2') {
				state = p1Down;
			}
		break;

		case p1Up:
			if(reset == 0x04) {
                                state = p1Static;
                                p1row = 0xF1;
                        }
			else if(x == '\0') {
                                state = p1Static;
                        }
                        else if(x == '1') {
                                state = p1Up;
                        }
                        else if(x == '2') {
                                state = p1Down;
                        }
		break;

		case p1Down:
			if(reset == 0x04) {
                                state = p1Static;
                                p1row = 0xF1;
                        }
			else if(x == '\0') {
                                state = p1Static;
                        }
                        else if(x == '4') {
                                state = p1Up;
                        }
                        else if(x == '5') {
                                state = p1Down;
                        }
		break;

		default:
			state = p1Static;
		break;
	}
	switch(state) {
		case p1Static:
		break;

		case p1Up:
			if(p1row != 0xF8) {
				p1row = (p1row >> 1) | 0x80;
			}
		break;

		case p1Down:
			if(p1row != 0xE3) {
				p1row = (p1row << 1) | 0x01;
			}
		break;

		default:
		break;
	}
	return state;
}

/*unsigned char aicolumn = 0x01;
unsigned char airow = 0xF1;

enum AIStates { aiStatic, aiUp, aiDown };

int AIFct(int state) {
	switch(state) {

	}
	switch(state) {

	}
	return state;
}*/

unsigned char p2column = 0x01;
unsigned char p2row = 0xF1;

enum Player2States { p2Static, p2Up, p2Down };

int Player2Fct(int state) {
	unsigned char bA = 0x00;
	bA = ~PINB & 0x03;
	switch(state) {
        	case p2Static:
			if(reset == 0x04) {
                                state = p2Static;
                                p2row = 0xF1;
                        }
			else if(bA == 0x01) {
				state = p2Up;
			}
			else if(bA == 0x02) {
				state = p2Down;
			}
			else if(bA != 0x01 || 0x02) {
				state = p2Static;
			}
			else {
				state = p2Static;
			}
		break;

		case p2Up:
			if(reset == 0x04) {
                                state = p2Static;
                                p1row = 0xF1;
                        }
			else if(bA == 0x01) {
                                state = p2Up;
                        }
                        else if(bA == 0x02) {
                                state = p2Down;
                        }
			else if(bA != 0x01 || 0x02) {
                                state = p2Static;
                        }
                        else {
                                state = p2Static;
                        }
		break;

		case p2Down:
			if(reset == 0x04) {
                                state = p2Static;
                                p1row = 0xF1;
                        }
			else if(bA == 0x01) {
                                state = p2Up;
                        }
                        else if(bA == 0x02) {
                                state = p2Down;
                        }
			else if(bA != 0x01 || 0x02) {
                                state = p2Static;
                        }
                        else {
                                state = p2Static;
                        }
		break;

		default:
			state = p2Static;
		break;
        }
        switch(state) {
		case p2Static:
		break;

		case p2Up:
			if(p2row != 0xF8) {
				p2row = (p2row >> 1) | 0x80;
			}
		break;

		case p2Down:
			if(p2row != 0xE3) {
				p2row = (p2row << 1) | 0x01;
			}
		break;

		default:
		break;
        }
        return state;
}

unsigned char ballcolumn = 0x08;
unsigned char ballrow = 0xFB;
unsigned char ballangle = 0x00;
unsigned char i = 0x00;
enum BallStates { ballWait, ballStart, ballLeft, ballRight, ballOne, ballTwo, RightUp, RightDown, LeftUp, LeftDown};
//	reset = ~PINB & 0x04;

int BallFct(int state) {
	switch(state) {
    		case ballWait:
			state = ballStart;
		break;	

		case ballStart:
			/*if(reset == 0x04) {
				state = ballReset;
			}
			else {*/
				state = ballLeft;
			//}
		break;
		
		/*case ballReset:
			if(reset == 0x04) {
				state = ballStart;
			}
			else {
				state = ballWait;
			}
		break;*/

		case ballLeft:
			/*if(reset == 0x04) {
				state = ballReset;
			}*/
			if(ballcolumn == 0x40) {
				if(ballrow == 0xFD && p1row == 0xF8) {
					state = ballRight;
				}
				else if(ballrow == 0xFB && p1row == 0xF1) {
					state = ballRight;
				}
				else if(ballrow == 0xF7 && p1row == 0xE3) {
					state = ballRight;
				}

				if(ballrow == 0xFE && p1row == 0xF8) {
					state = RightUp;
					ballangle = 0x01;
				}
				else if(ballrow == 0xFD && p1row == 0xF1) {
					state = RightUp;
					ballangle = 0x02;
				}
				else if(ballrow == 0xFB && p1row == 0xE3) {
					state = RightUp;
					ballangle = 0x03;
				}

				if(ballrow == 0xFB && p1row == 0xF8) {
					state = RightDown;
					ballangle = 0x01;
				}
				else if(ballrow == 0xF7 && p1row == 0xF1) {
					state = RightDown;
					ballangle = 0x02;
				}
				else if(ballrow == 0xEF && p1row == 0xE3) {
					state = RightDown;
					ballangle = 0x03;
				}
			}
			else {
				if(ballcolumn == 0x80) {
					state = ballOne;
				}
				else {
					state = ballLeft;
				}
			}
		break;

		case ballRight:
			/*if(reset == 0x04) {
				state = ballReset;
			}*/
			if(ballcolumn == 0x02) {
                                 if(ballrow == 0xFD && p2row == 0xF8) {
                                         state = ballLeft;
                                 }
                                 else if(ballrow == 0xFB && p2row == 0xF1) {
                                         state = ballLeft;
                                 }
                                 else if(ballrow == 0xF7 && p2row == 0xE3) {
                                         state = ballLeft;
                                 }
 
                                 if(ballrow == 0xFE && p2row == 0xF8) {
                                         state = LeftUp;
					 ballangle = 0x01;
                                 }
                                 else if(ballrow == 0xFD && p2row == 0xF1) {
                                         state = LeftUp;
					 ballangle = 0x02;
                                 }
                                 else if(ballrow == 0xFB && p2row == 0xE3) {
                                         state = LeftUp;
					 ballangle = 0x03;
                                 }
 
                                 if(ballrow == 0xFB && p2row == 0xF8) {
                                         state = LeftDown;
					 ballangle = 0x01;
                                 }
                                 else if(ballrow == 0xF7 && p2row == 0xF1) {
                                         state = LeftDown;
					 ballangle = 0x02;
                                 }
                                 else if(ballrow == 0xEF && p2row == 0xE3) {
                                         state = LeftDown;
					 ballangle = 0x03;
                                 }
			}
			else {
				if(ballcolumn == 0x01) {
					state = ballTwo;
				}
				else {
					state = ballRight;
				}
			}
		break;

		case ballOne:
				state = ballLeft;
		break;

		case ballTwo:
				state = ballRight;
		break;

		case RightUp:
				if(ballrow == 0xEF) { //possibly change for ballcolumn
					state = RightDown;
				}
				else {
					state = ballRight;
				}
		break;

		case RightDown:
				if(ballrow == 0xFE) {
					state = RightUp;
				}
				else {
					state = ballRight;
				}
		break;

		case LeftUp:
				if(ballrow == 0xEF) {
					state = LeftDown;
				}
				else {
					state = ballLeft;
				}
		break;

		case LeftDown:
				if(ballrow == 0xFE) {
					state = LeftUp;
				}
				else {
					state = ballLeft;
				}	
		break;

		default:
			state = ballWait;
		break;
        }

        switch(state) {
        	case ballWait:
		break;

		case ballStart:
		break;

		/*case ballReset:
			ballrow = 0xFB;
                        ballcolumn = 0x08;
		break;*/
			
		case ballLeft:
			ballcolumn = ballcolumn << 1;
		break;

		case ballRight:
			ballcolumn = ballcolumn >> 1;
		break;

		case ballOne:
			ballcolumn = 0x08;
                        ballrow = 0xFB;

		break;

		case ballTwo:
			ballcolumn = 0x10;
                        ballrow = 0xFB;
		break;

		case RightUp:
				/*if(ballangle == 0x01) {
					ballcolumn = 0x40;
					ballrow = 0xFD;
				}
				else if(ballangle == 0x02) {
					ballcolumn = 
				}
				else if(ballangle == 0x03) {

				}*/
			while(RightUp) {
                                ballcolumn = ballcolumn >> 1;
                                ballrow = ballrow >> 1;
                        }

		break;

		case RightDown:
			while(RightDown) {
				ballcolumn = ballcolumn >> 1;
				ballrow = ballrow << 1;
			}
		break;

		case LeftUp:
			while(LeftUp) {
				ballcolumn = ballcolumn << 1;
				ballrow = ballrow >> 1;
			}
		break;

		case LeftDown:
			while(LeftDown) {
				ballcolumn = ballcolumn << 1;
				ballrow = ballrow << 1;
			}
		break;
	}	
        return state;
}

unsigned char columns[5];
unsigned char rows[5];

enum combineLED { displayStart, displayLED };

int displayFct(int state) {
	static unsigned char i;
	static unsigned char count = 0;
	switch(state) {
		case displayStart:
			state = displayLED;
		break;

        	case displayLED:
			if(count == 5) { 
				count = 0;
			}
			else {
				++count;
			}
			for (i = 0; i < 5; ++i) {
				columns[i] = 0;
					if((ballrow & (0x01 << i)) == 0) {
						columns[i] = ballcolumn;
					}
					if((p1row & (0x01 << i)) == 0) {
						columns[i] = columns[i] | p1column;
					}
					if((p2row & (0x01 << i)) == 0) {
						columns[i] = columns[i] | p2column;
					}

					/*if((ballrow & (0x01 << i)) == 0) { //== 0x00
						columns[i] = ballcolumn; //ball movement
					}
					if((p1row & (0x01 << i)) == 0) { //player 1 paddle
						columns[i] |= p1column;
					}
					if((p2row & (0x01 << i)) == 0) { //player 2 paddle
                                                columns[i] |= p2column;
                                        }*/
					if(((ballrow & p1row & p2row) & (0x01 << i)) == 0) { //displays paddles and ball
						rows[i] = ~(0x01 << i);
					}
			}
		break;

		default:
			state = displayLED;
		break;
        }
        switch(state) { 
		case displayLED:
			PORTC = columns[count];
			PORTD = rows[count];
		break;

		default:
		break;
        }
        return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0x00; PORTB = 0xFF; 
	DDRC = 0xFF; PORTC = 0xFF; 
	DDRD = 0x1F; PORTD = 0x80;
    /* Insert your solution below */
	static task task1, task2, task3, task4;
	task *tasks[] = {&task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	const char start = -1;

	task1.state = start;
	task1.period = 150;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Player1Fct; //assign

	task2.state = start;
        task2.period = 150;
        task2.elapsedTime = task2.period;
        task2.TickFct = &Player2Fct;

	task3.state = start;
        task3.period = 150;
        task3.elapsedTime = task3.period;
        task3.TickFct = &BallFct;

	task4.state = start;
        task4.period = 1;
        task4.elapsedTime = task4.period;
        task4.TickFct = &displayFct;


	unsigned long GCD = tasks[0]->period;
	for(unsigned long i = 1; i < numTasks; i++) {
        	GCD = findGCD(GCD, tasks[i]->period);
	}

	TimerSet(GCD);
	TimerOn();

    while (1) {
	    for(unsigned long i = 0; i < numTasks; i++) {
		if(tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += GCD;
	    }
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
