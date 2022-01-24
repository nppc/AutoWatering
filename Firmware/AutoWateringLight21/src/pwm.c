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

void turnLedsOn(uint16_t spd){
  setval_PWMout(0,LIGHTPANELPWM_MAX); // turn lights on
  setval_PWMout(1,LIGHTPANELPWM_MAX); // turn lights on
  setval_PWMout(2,LIGHTPANELPWM_MAX); // turn lights on
  pwmglob.lightpanelspeed = spd;
}

void turnLedsOff(uint16_t spd){
  setval_PWMout(0,LIGHTPANELPWM_MIN); // turn lights on
  setval_PWMout(1,LIGHTPANELPWM_MIN); // turn lights on
  setval_PWMout(2,LIGHTPANELPWM_MIN); // turn lights on
  pwmglob.lightpanelspeed = spd;
}
