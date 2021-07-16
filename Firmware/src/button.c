//#pragma src
// Button routines
#include "button.h"

volatile int16_t buttoncntr;
volatile BUT_STATE buttonstate;

uint8_t getButtonState(void){
	uint8_t tmp = buttonstate;
	uint16_t tmpcntr;
	if(buttonstate==BUT_PRESSED){
		// button is pressed
	    IE_EA = 0;     //Disable interrupts
	    tmpcntr = buttoncntr;
	    IE_EA = 1;      //Enable interrupts
	  if(tmpcntr >= 161) {
			//glob.calibselection = (tmpcntr-161)/64;
		}else{
		  //glob.calibselection = 0;
			return BUT_NOTPRESSED;
		}
		return BUT_PRESSED;
	}
	buttonstate = BUT_NOTPRESSED;
	return tmp;
}
