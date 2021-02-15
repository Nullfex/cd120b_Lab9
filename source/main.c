/*	Author: lab9
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include "timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char combined;
unsigned char speakerval;
double countthree;
double countblink;
double countspeaker;
double countbutton;
double speakerspeed;
char aval;

enum States{threestart,one,two,three}threestate;
enum BlinkStates{blinkstart,on,off}blinkstate;
enum CombineStates{combinestart,calc}combinestate;
enum speakerStates{speakerstart,speakeron,speakeroff}speakerstate;
enum buttonstate{buttonstart,nopress,increase,decrease}buttonstate;

void buttonSM(){
	
	switch(buttonstate){
		case buttonstart:
		countbutton = 0;
		buttonstate = nopress;
		break;

		case nopress:
		countbutton = 0;
		aval = ~PINA&0x03;
		if(aval == 0x01){
		if(speakerspeed<=1000){
		speakerspeed++;
		}	
		buttonstate = increase;
		}else if(aval == 0x02){
		if(speakerspeed>=2){
		speakerspeed--;
		}	
		buttonstate = decrease;
		}else{
		buttonstate = nopress;
		}

		case increase:
		aval = ~PINA&0x03;
		if(aval == 0){
		buttonstate = nopress;
		}else{
		buttonstate = increase;
		}

		case decrease:
                aval = ~PINA&0x03;
                if(aval == 0){
                buttonstate = nopress;
                }else{
                buttonstate = decrease;
                }
	}
}

void ThreeLEDsSM(){

	switch(threestate){
		case threestart:
		threeLEDs = 0;
		countthree = 0;
		threestate = one;	
		break;

		case one:
		if(countthree>=300){
		countthree = 0;	
		threestate = two;
		}else{
		threestate = one;
		countthree++;
		}
		threeLEDs = 0x01;
		break;
		
		case two:
		if(countthree>=300){
		countthree = 0;	
		threestate = three;
		}else{
		threestate = two;
		countthree++;
		}
		threeLEDs = 0x02;
		break;

		case three:
		if(countthree>=300){
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

void speakerSM(){
        switch(speakerstate){
                case speakerstart:
		countspeaker = 0;	
		speakerval = 0x00;
		speakerstate = speakeroff;
		break;

		case speakeroff:
		speakerval = 0;
		aval = ~PINA&0x04;
		if(aval==0x04){
		if(countspeaker>=speakerspeed){
		countspeaker = 0;
		speakerstate = speakeron;
		}else{
		speakerstate = speakeroff;
		countspeaker++;
		}
		}else{
		speakerstate = speakeroff;
		countspeaker = 0;
		}
		break;

		case speakeron:
		aval = ~PINA&0x04;
		if(aval==0x04){	
		speakerval = 0x01;
		if(countspeaker>=speakerspeed){
		countspeaker = 0;
		speakerstate = speakeroff;
		}else{
		speakerstate = speakeron;
		countspeaker++;	
		}
		}else{
		speakerstate = speakeroff;
		countspeaker = 0;
		speakerval = 0;
		}
        }

}


void CombineLEDsSM(){

	switch(combinestate){
		case combinestart:
		combinestate = calc;
		PORTB = 0x00;
		break;

		case calc:
		combined =(speakerval<<4)|(threeLEDs<<1)|blinkingLED;
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
	speakerspeed = 2;
	threestate = threestart;
	blinkstate = blinkstart;
	combinestate = combinestart;
	speakerstate = speakerstart;
	buttonstate = buttonstart;
	TimerSet(1);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	ThreeLEDsSM();
    	BlinkingLEDSM();
	buttonSM();
	speakerSM();
	CombineLEDsSM();	
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
