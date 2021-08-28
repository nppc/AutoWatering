//#pragma src
// Button routines
#include "button.h"

volatile int16_t buttoncntr;
volatile BUT_STATE buttonstate = BUT_NOTPRESSED;

// Interrupts can set buttonstate to one of the states: BUT_PRESSED, BUT_LONGPRESS, BUT_SHORTPRESS,
// This routine reads that statuses and returns them allowing kind of memory effect.
// After status is read the status updated to BUT_NOTPRESSED.
uint8_t getButtonState(void){
	uint8_t tmp = buttonstate; // preserve button state
	uint16_t tmpcntr;
	buttonstate = BUT_NOTPRESSED; // next time we need to return no press
	if(tmp==BUT_PRESSED){
		// button is pressed, let's see for how long time...
	    IE_EA = 0;     //Disable interrupts
	    tmpcntr = buttoncntr;
	    IE_EA = 1;      //Enable interrupts
		if(tmpcntr >= 20*2) tmp = BUT_PRESSED2S; // about 1.5s
		if(tmpcntr >= 32*5) tmp = BUT_PRESSED5S;
	}
	return tmp;
}
