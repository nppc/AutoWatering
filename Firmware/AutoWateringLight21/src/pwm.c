#include "main.h"
#include "pwm.h"

/*
void enable_PWMout0(void){
  PCA0CPM0 |= PCA0CPM0_ECOM__ENABLED; // enable led PWM
  P0MDOUT |= P0MDOUT_B7__PUSH_PULL; // Set pushpull
}

void enable_PWMout1(void){
	PCA0CPM1 |= PCA0CPM1_ECOM__ENABLED; // enable led PWM
	P1MDOUT |= P1MDOUT_B0__PUSH_PULL;	// Set pushpull
}

void enable_PWMout2(void){
	PCA0CPM2 |=PCA0CPM2_ECOM__ENABLED; // enable led PWM
	P1MDOUT |= P1MDOUT_B1__PUSH_PULL;	// Set pushpull
}
*/

void setval_PWMout(int8_t ch, int16_t val){
  if(val <= LIGHTPANELPWM_MAX && val >=LIGHTPANELPWM_MIN){
    IE_EA = 0;
	pwmglob.pwmchangecntr = 0; // change during first Timer2 Interrupt
    pwmglob.set_out[ch] = val;
    IE_EA = 1;
  }
}

/*
void disable_PWMout0(void){
  P0MDOUT &= ~P0MDOUT_B7__BMASK;  // Set opendrain
  PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK; // enable led PWM
  PIN_PWM0 = 0;
}

void disable_PWMout1(void){
	P1MDOUT &= ~P1MDOUT_B0__BMASK;	// Set opendrain
	PCA0CPM1 &= ~PCA0CPM1_ECOM__BMASK; // enable led PWM
	PIN_PWM1 = 0;
}

void disable_PWMout2(void){
	P1MDOUT &= ~P1MDOUT_B1__BMASK;	// Set opendrain
	PCA0CPM2 &= ~PCA0CPM2_ECOM__BMASK; // enable led PWM
	PIN_PWM2 = 0;
}
*/
