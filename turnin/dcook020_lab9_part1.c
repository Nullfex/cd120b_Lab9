/*	Author: lab9
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=bj6AKQLjQ-Y
 */
#include "timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"

unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char combined;
double countthree;
double countblink;

enum States{threestart,one,two,three}threestate;
enum BlinkStates{blinkstart,on,off}blinkstate;
enum CombineStates{combinestart,calc}combinestate;

void ThreeLEDsSM(){

	switch(threestate){
		case threestart:
		threeLEDs = 0;
		countthree = 0;
		threestate = one;	
		break;

		case one:
		if(countthree>=1000){
		countthree = 0;	
		threestate = two;
		}else{
		threestate = one;
		countthree++;
		}
		threeLEDs = 0x01;
		break;
		
		case two:
		if(countthree>=1000){
		countthree = 0;	
		threestate = three;
		}else{
		threestate = two;
		countthree++;
		}
		threeLEDs = 0x02;
		break;

		case three:
		if(countthree>=1000){
		countthree= 0;	
		threestate = one;
		}else{
		threestate = three;
		countthree++;
		}
		threeLEDs = 0x04;
		break;
	}


}

void BlinkingLEDSM(){
	
	switch(blinkstate){
		case blinkstart:
		blinkingLED = 0;
		countblink = 0;
		blinkstate = on;
		break;

		case on:
		if(countblink>=1000){
		blinkstate = off;
		countblink = 0;
		}else{
		blinkstate = on;
		countblink++;	
		}
		blinkingLED = 0x01;
		break;

		case off:
		if(countblink>=1000){
		blinkstate = on;
		countblink = 0;
		}else{
		blinkstate = off;
		countblink++;	
		}
		blinkingLED = 0x00;
		break;
	
	}

}


void CombineLEDsSM(){

	switch(combinestate){
		case combinestart:
		combinestate = calc;
		PORTB = 0x00;
		break;

		case calc:
		combined =(threeLEDs<<1)|blinkingLED;
		PORTB = combined;
		combinestate = calc;
		break;
	}

}


int main(void) {
    /* Insert DDR and PORT initializations */
DDRB = 0xFF; PORTB = 0x00;
DDRA = 0x00; PORTA = 0xFF;
	threeLEDs=0;
	blinkingLED=0;
	combined=0;

	threestate = threestart;
	blinkstate = blinkstart;
	combinestate = combinestart;
	TimerSet(1);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	ThreeLEDsSM();
    	BlinkingLEDSM();
	CombineLEDsSM();	
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
