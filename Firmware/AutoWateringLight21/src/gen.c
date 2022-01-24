//#pragma src
#include "main.h"
#include "gen.h"
#include "adc.h"
#include "pwm.h"
#include "SSD1306.h"
//#include "bitmaps.h"

volatile uint16_t tmp_millis;
volatile bit delay_on;
volatile uint8_t scrolldelay;


void delay_ms(uint16_t ms_count)
 {
  tmp_millis = ms_count;
  delay_on = 1;
  while(delay_on);
 }

// round result to the nearest integer
uint16_t div32round(uint32_t d1, uint32_t d2){
  return (d1 + (d2 / 2)) / d2;
}

bit run_timers(void){
	bit retval=0;
	if(ms_tick){
	    U16_U8 tmp;
	    ms_tick = false;

	    if(pwmglob.pwmchangecntr==0){
	    pwmglob.pwmchangecntr = pwmglob.lightpanelspeed;
	      if(pwmglob.cur_out[0]<pwmglob.set_out[0]){pwmglob.cur_out[0]++;pwmOut0_update = 1;} // adjust
	      else if(pwmglob.cur_out[0]>pwmglob.set_out[0]){pwmglob.cur_out[0]--;pwmOut0_update = 1;} // adjust
	      if(pwmglob.cur_out[1]<pwmglob.set_out[1]){pwmglob.cur_out[1]++;pwmOut1_update = 1;} // adjust
	      else if(pwmglob.cur_out[1]>pwmglob.set_out[1]){pwmglob.cur_out[1]--;pwmOut1_update = 1;} // adjust
	      if(pwmglob.cur_out[2]<pwmglob.set_out[2]){pwmglob.cur_out[2]++;pwmOut2_update = 1;} // adjust
	      else if(pwmglob.cur_out[2]>pwmglob.set_out[2]){pwmglob.cur_out[2]--;pwmOut2_update = 1;} // adjust

	      if(pwmOut0_update){
	        // disable output if needed
	        if(pwmglob.cur_out[0]==LIGHTPANELPWM_MIN){
	          P0MDOUT &= ~P0MDOUT_B7__BMASK;  // Set opendrain
	          PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK; // disable led PWM
	          PIN_PWM0 = 0;
	        }
	        PCA0PWM |= PCA0PWM_ARSEL__AUTORELOAD;
	        tmp.u16 = pwmglob.cur_out[0];
	        // convert 16bit to 2x8bit
	        PCA0CPL0 = tmp.u8[1];
	        PCA0CPH0 = tmp.u8[0];
	        pwmOut0_update = 0;
	        // enable output if needed
	        if(pwmglob.cur_out[0]==(LIGHTPANELPWM_MIN+1) && pwmglob.set_out[0]>LIGHTPANELPWM_MIN){
	          PCA0CPM0 |= PCA0CPM0_ECOM__ENABLED; // enable led PWM
	          P0MDOUT |= P0MDOUT_B7__PUSH_PULL; // Set pushpull
	        }
	      }

	      if(pwmOut1_update){
	        if(pwmglob.cur_out[1]==LIGHTPANELPWM_MIN){
	          P1MDOUT &= ~P1MDOUT_B0__BMASK;  // Set opendrain
	          PCA0CPM1 &= ~PCA0CPM1_ECOM__BMASK; // enable led PWM
	          PIN_PWM1 = 0;
	        }
	        PCA0PWM |= PCA0PWM_ARSEL__AUTORELOAD;
	        tmp.u16 = pwmglob.cur_out[1];
	        // convert 16bit to 2x8bit
	        PCA0CPL1 = tmp.u8[1];
	        PCA0CPH1 = tmp.u8[0];
	        pwmOut1_update = 0;
	        // enable output if needed
	        if(pwmglob.cur_out[1]==(LIGHTPANELPWM_MIN+1) && pwmglob.set_out[1]>LIGHTPANELPWM_MIN){
	          PCA0CPM1 |= PCA0CPM1_ECOM__ENABLED; // enable led PWM
	          P1MDOUT |= P1MDOUT_B0__PUSH_PULL; // Set pushpull
	        }
	      }

	      if(pwmOut2_update){
	        if(pwmglob.cur_out[2]==LIGHTPANELPWM_MIN){
	          P1MDOUT &= ~P1MDOUT_B1__BMASK;  // Set opendrain
	          PCA0CPM2 &= ~PCA0CPM2_ECOM__BMASK; // enable led PWM
	          PIN_PWM2 = 0;
	        }
	        PCA0PWM |= PCA0PWM_ARSEL__AUTORELOAD;
	        tmp.u16 = pwmglob.cur_out[2];
	        // convert 16bit to 2x8bit
	        PCA0CPL2 = tmp.u8[1];
	        PCA0CPH2 = tmp.u8[0];
	        pwmOut2_update = 0;
	        // enable output if needed
	        if(pwmglob.cur_out[2]==(LIGHTPANELPWM_MIN+1) && pwmglob.set_out[2]>LIGHTPANELPWM_MIN){
	          PCA0CPM2 |=PCA0CPM2_ECOM__ENABLED; // enable led PWM
	          P1MDOUT |= P1MDOUT_B1__PUSH_PULL; // Set pushpull
	        }
	      }
	    }else{
	    pwmglob.pwmchangecntr--;
	  }
	}

	if(second_tick){
		// occurs every second
    retval=1;

    second_tick=0;
		
		if(glob.dayphase_cntr_s < 240) glob.dayphase_cntr_s++; // counter for dayphase change (max 4 minutes)
		
		glob.screenSaver_s--;
		
		// advance daylight counter
		if(glob.ledOnOff_cntr_s>0) glob.ledOnOff_cntr_s--;


		// read LED light
		ADC_readLight = 1;

    // read temperature every 9s
    if(adcglob.temp_read_s>0){
        adcglob.temp_read_s--;
    }else{
        ADC_readTemp = 1;
        adcglob.temp_read_s = 9; //seconds to wait
    }

		switch (glob.machinestate){
			case MACHINE_WAIT:
				if(glob.p_wait_sub_s>0){
					glob.p_wait_sub_s--;
				}else{
					glob.p_wait_sub_s = 60;
					if(glob.p_wait_cntr_m>0) glob.p_wait_cntr_m--;
				}
				break;
			case MACHINE_RUN:
				if(glob.p_run_cntr_s>0) glob.p_run_cntr_s--;
				break;
		}
	}
	return retval;
}

// format: XXm XXs
void show_time_s(uint8_t row, uint16_t num){
	int16_t tmp;
	tmp = num;
	ssd1306_printTimeM(22,row,tmp / 60);
	tmp = tmp % 60;
	ssd1306_printTimeS(62,row,tmp);
}

// format: XXh XXm
void show_time_m(uint8_t row, uint16_t num){
	int16_t tmp;
	tmp = num;
  ssd1306_printTimeH(22,row,tmp / 60);
  tmp = tmp % 60;
  ssd1306_printTimeM(57,row,tmp);
}

void scroll_image(void){
	if(scrolldelay==0 && glob.machinestate==MACHINE_RUN){
		// time to scroll
		scrolldelay=70; // ms
		scroll_down(11, 2);
		ssd1306_printBitmapX(0, 2, 11, 2, scrlbuff);
	}
}

// Return undefined, cloud or sun according to LED light sensor readings
DAYPHASE getDayPhase(void){
  DAYPHASE newdayphase = glob.dayphase;
  
  if(glob.Vlight<LEDSENSOR_SUN && glob.dayphase==DAYPHASE_SUN) newdayphase = DAYPHASE_NOSUN;
  else if(glob.Vlight>=LEDSENSOR_SUN && glob.dayphase==DAYPHASE_NOSUN) newdayphase = DAYPHASE_SUN;
  
  // stabilize the sun/cloud
  // start timer if dayphase was changed
  if(glob.dayphase!=newdayphase) glob.dayphase_cntr_s = 0; // counter will increment

  return newdayphase;
}

uint16_t getWaitValue(void){
  return eeprom_data[0].p_wait;
}

// if daylight counter is 0 then we can turn Lights on or off according to dayphase
// if sunny, then Lights should be off
// also turn lights off if daylight counter is expired, but there is still not dark outside.
void setLightsOnOff(void){
  bit l_cur = (pwmglob.set_out[0] == LIGHTPANELPWM_MIN ? false : true);
//  if(glob.dayphase == DAYPHASE_UNDEFINED) return; // do nothing if dayphase is not stabilized
  
  if(glob.ledOnOff_cntr_s==0){  // timer expired - turn leds on or off
      // When timer expiredm turn on/off slowly
      needLight = !needLight; // switch mode
      if(needLight){
          turnLedsOn(LIGHTPANELSPEEDSLOW);
          glob.ledOnOff_cntr_s = eeprom_data[0].ontime;
      }else{
          turnLedsOff(LIGHTPANELSPEEDSLOW);
          glob.ledOnOff_cntr_s = eeprom_data[0].offtime;
      }
  }else if(needLight){
      // if during the day dayphase changes - turn on/off fast
      DAYPHASE curphase = glob.dayphase;
      glob.dayphase = getDayPhase();
      if(curphase!=glob.dayphase && glob.dayphase_cntr_s>60){
          // turn on or off fast
          if(glob.dayphase == DAYPHASE_NOSUN) turnLedsOn(LIGHTPANELSPEEDFAST); else turnLedsOff(LIGHTPANELSPEEDFAST);
      }

  }

}

