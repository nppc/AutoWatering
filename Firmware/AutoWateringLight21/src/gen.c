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
		if(glob.daylight_cntr_s>0) glob.daylight_cntr_s--;


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

// Return night, cloud or sun according to LED light sensor readings
DAYPHASE getDayPhase(void){
  DAYPHASE newdayphase = glob.dayphase;
  
  if(glob.Vlight<=LEDSENSOR_NIGHT && daylight) newdayphase = DAYPHASE_NIGHT;
  else if(glob.Vlight<LEDSENSOR_SUN && glob.dayphase==DAYPHASE_SUN) newdayphase = DAYPHASE_CLOUD;
  else if(glob.Vlight>(daylight ? LEDSENSOR_NIGHT : LEDSENSOR_MORNING) && glob.dayphase==DAYPHASE_NIGHT) newdayphase = DAYPHASE_CLOUD;
  else if(glob.Vlight>=LEDSENSOR_SUN && glob.dayphase==DAYPHASE_CLOUD) newdayphase = DAYPHASE_SUN;
  else if(glob.Vlight<(LEDSENSOR_SUN) && glob.dayphase==DAYPHASE_SUN) newdayphase = DAYPHASE_CLOUD;
  
  // start timer if dayphase was changed
  if(glob.dayphase!=newdayphase) glob.dayphase_cntr_s = 0; // counter will increment

  return newdayphase;
}

uint16_t getWaitValue(void){
  if(glob.dayphase==DAYPHASE_NIGHT) return eeprom_data[0].p_wait_night; 
  if(glob.dayphase==DAYPHASE_SUN) return eeprom_data[0].p_wait_sun; 
  return eeprom_data[0].p_wait_cloud;
}

// if daylight counter is 0 then we can turn Lights on or off according to dayphase
// if sunny, then Lights should be off
// also turn lights off if daylight counter is expired, but there is still not dark outside.
void setLightsOnOff(void){
  bit l_on = 0;
  if(glob.dayphase_cntr_s < 60) return; // do nothing if dayphase is not stabilized

  if(daylight && glob.daylight_cntr_s>0){
      if(glob.dayphase!=DAYPHASE_SUN) l_on = 1;
  }
  
  if(l_on && pwmglob.set_out[0] == LIGHTPANELPWM_MIN) setval_PWMout(0,LIGHTPANELPWM_MAX); // turn lights on
  else if(l_on==0 && pwmglob.set_out[0] == LIGHTPANELPWM_MAX) setval_PWMout(0,LIGHTPANELPWM_MIN); // turn lights off

  if(l_on && pwmglob.set_out[1] == LIGHTPANELPWM_MIN) setval_PWMout(1,LIGHTPANELPWM_MAX); // turn lights on
  else if(l_on==0 && pwmglob.set_out[1] == LIGHTPANELPWM_MAX) setval_PWMout(1,LIGHTPANELPWM_MIN); // turn lights off

  if(l_on && pwmglob.set_out[2] == LIGHTPANELPWM_MIN) setval_PWMout(2,LIGHTPANELPWM_MAX); // turn lights on
  else if(l_on==0 && pwmglob.set_out[2] == LIGHTPANELPWM_MAX) setval_PWMout(2,LIGHTPANELPWM_MIN); // turn lights off
}

// switch to daylight mode if counter is 0 or there is cloudy or sunny weather longer than 1 minute
// After startup we do not know where we in timeline. So, this is special case.
// Only process the logic if dayphase stabilized (longer than 1 minute)
//   Startup (variable startup=1):
//   If it is night, start the timer - we in the night.
//   If it is cloudy or sunny, start the timer, but switch to the night as soon as there is no light. Then start night timer. 
// Normal case:
//   if we in daylight and it is night outside and daylight timer is expired, then go to night mode 
//   if we in night mode and night mode timer is expired and there is no night outside - switch to daylight mode
//   TODO: do we need to turn lights off if daylight timer is expired, but no night outside (anyway this will be done outside this routine)?
void setDaylight(void){
  if(glob.dayphase_cntr_s < 60) return; // do nothing if dayphase is not stabilized
  if(startup){
	// startup case
	if(glob.dayphase==DAYPHASE_NIGHT){
		// we in night - go to night mode and end startup phase
		daylight = 0;
		glob.daylight_cntr_s = eeprom_data[0].night; 
		startup = 0;
	}else if(daylight==0 && glob.dayphase!=DAYPHASE_NIGHT){
		daylight = 1;
		glob.daylight_cntr_s = eeprom_data[0].daylight;
		// we still continue in startup
	}
	// we are in daylight, but not sure how long. So, switch to night mode as soon as there is no light outside (first check)
  }else{
	// normal case
	if(daylight && glob.dayphase==DAYPHASE_NIGHT && glob.daylight_cntr_s==0){
		// go to night
		daylight = 0;
		glob.daylight_cntr_s = eeprom_data[0].night;
	}else if(daylight==0 && glob.dayphase!=DAYPHASE_NIGHT && glob.daylight_cntr_s==0){
		// go to daylight
		daylight = 1;
		glob.daylight_cntr_s = eeprom_data[0].daylight;
	}
	
  }
  
}

void setLigthPanelSpeed(void){
  if(daylight){
      // switch to fast after we slowly turned panel fully on.
      if(pwmglob.lightpanelspeed == LIGHTPANELSPEEDSLOW && pwmglob.cur_out[0] == LIGHTPANELPWM_MAX) pwmglob.lightpanelspeed = LIGHTPANELSPEEDFAST;
  }else{
      // at night we always can be at slow
      pwmglob.lightpanelspeed = LIGHTPANELSPEEDSLOW;
  }
}
