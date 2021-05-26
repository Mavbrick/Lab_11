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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "bit.h"
#include "scheduler.h"
#include "keypad.h"
#endif

/* implement the SMs */

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;	//change these, setup to A,B,C,D
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0x00; PORTD = 0xFF;
    /* Insert your solution below */
	//only 4 tasks so far
	static task task1, task2, task3, task4 ; //setup to 4 tasks atm
	task *tasks[] = {&task1, &task2, &task3, &task4,};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	const char start = -1;

	task1.state = abc;	//change initial states
	task1.period = 1;	//change periods
	task1.elapsedTime = task1.period;
	task1.TickFct = &SM1;	//change SM names

	task2.state = def;
	task2.period = 1;
	task2.elapsedTime = task2.period;
	task2.TickFct = &SM2;

	task3.state = ghi;
	task3.period = 1;
	task3.elapsedTime = task3.period;
	task3.TickFct = &SM3;

	task4.state = jkl;
        task4.period = 1;
        task4.elapsedTime = task4.period;
        task4.TickFct = &SM4;

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
