#include "main.h"
#include "pwm.h"

/*
void inc_PWMout1(int16_t val){
	val = pwmglob.set_out1 + val;
	if(val>PWMMAXOUT1) val = PWMMAXOUT1;
	setval_PWMout1(val);
}

void dec_PWMout1(int16_t val){
	val = pwmglob.set_out1 - val;
	if(val<PWMMINOUT1) val = PWMMINOUT1;
	setval_PWMout1(val);
}

void inc_PWMout2(int16_t val){
	val = pwmglob.set_out2 + val;
	if(val>PWMMAXOUT2) val = PWMMAXOUT2;
	setval_PWMout2(val);
}

void dec_PWMout2(int16_t val){
	val = pwmglob.set_out2 - val;
	if(val<PWMMINOUT2) val = PWMMINOUT2;
	setval_PWMout2(val);
}
*/

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


void setval_PWMout0(int16_t val){
  IE_EA = 0;
  pwmglob.set_out[0] = val;
  pwmOut0_update = 1;
  //PCA0CPM0 |=PCA0CPM0_ECCF__BMASK;
  //PCA0CN0_CCF0=1; // force update
  IE_EA = 1;
}

void setval_PWMout1(int16_t val){
	IE_EA = 0;
	pwmglob.set_out[1] = val;
	pwmOut1_update = 1;
	//PCA0CPM1 |=PCA0CPM1_ECCF__BMASK;
	//PCA0CN0_CCF1=1; // force update
	IE_EA = 1;
}

void setval_PWMout2(int16_t val){
	IE_EA = 0;
	pwmglob.set_out[2] = val;
	pwmOut2_update = 1;
//	PCA0CPM2 |=PCA0CPM2_ECCF__BMASK;
//	PCA0CN0_CCF2=1; // force update
	IE_EA = 1;
}

/*
void setval_PWMout(int8_t ch, int16_t val){
  IE_EA = 0;
  pwmglob.set_out[ch] = val;
  switch (ch){
      case 0:
        pwmOut0_update = 1;
        PCA0CPM0 |=PCA0CPM0_ECCF__BMASK;
        PCA0CN0_CCF0=1; // force update
        break;
      case 1:
        pwmOut1_update = 1;
        PCA0CPM1 |=PCA0CPM1_ECCF__BMASK;
        PCA0CN0_CCF1=1; // force update
        break;
      case 2:
        pwmOut2_update = 1;
        PCA0CPM2 |=PCA0CPM2_ECCF__BMASK;
        PCA0CN0_CCF2=1; // force update
        break;
  }
  IE_EA = 1;
}
*/

void disable_PWMout0(void){
  P0MDOUT &= !P0MDOUT_B7__BMASK;  // Set opendrain
  PCA0CPM1 |=PCA0CPM1_ECOM__DISABLED; // enable led PWM
  PIN_PWM0 = 0;
}

void disable_PWMout1(void){
	P1MDOUT &= !P1MDOUT_B0__BMASK;	// Set opendrain
	PCA0CPM1 |=PCA0CPM1_ECOM__DISABLED; // enable led PWM
	PIN_PWM1 = 0;
}

void disable_PWMout2(void){
	P1MDOUT &= !P1MDOUT_B1__BMASK;	// Set opendrain
	PCA0CPM2 |=PCA0CPM2_ECOM__DISABLED; // enable led PWM
	PIN_PWM2 = 0;
}

