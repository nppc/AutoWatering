#include "main.h"
#include "pwm.h"


void setval_PWMout(int8_t ch, int16_t val){
  if(val <= LIGHTPANELPWM_MAX && val >=LIGHTPANELPWM_MIN){
    IE_EA = 0;
	pwmglob.pwmchangecntr = 0; // change during first Timer2 Interrupt
    pwmglob.set_out[ch] = val;
    IE_EA = 1;
  }
}

