//#pragma src
#include "main.h"
#include "gen.h"
#include "adc.h"
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
	if(second_tick){
		second_tick=0;
		
		glob.screenSaver_s--;
		
    // advance daylight counter
    if(glob.daylight_cntr_s>0) glob.daylight_cntr_s--;


		// read LED light every 20s
		if(adcglob.led_read_s>0){
		    adcglob.led_read_s--;
		}else{
		    ADC_readLED = 1;
		    adcglob.led_read_s = 20; //seconds to wait
        retval=1;
		}

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
					retval=1;
				}
				break;
			case MACHINE_RUN:
				if(glob.p_run_cntr_s>0) glob.p_run_cntr_s--;
				retval=1;
				break;
		}
	}
	return retval;
}

// format: XXm XXs
void show_time_s(uint16_t num){
	int16_t tmp;
	tmp = num;
	ssd1306_printTimeM(22,0,tmp / 60);
	tmp = tmp % 60;
	ssd1306_printTimeS(64,0,tmp);
}

// format: XXh XXm
void show_time_m(uint16_t num){
	int16_t tmp;
	tmp = num;
	ssd1306_printTimeH(22,0,tmp / 60);
	tmp = tmp % 60;
	ssd1306_printTimeM(59,0,tmp);
}

void scroll_image(void){
	if(scrolldelay==0 && glob.machinestate==MACHINE_RUN){
		// time to scroll
		scrolldelay=70; // ms
		scroll_down(11, 2);
		ssd1306_printBitmapX(0, 0, 11, 2, scrlbuff);
	}
}


void configAdjustValue(CONFIG_STATE state){
  uint8_t t;
  switch (state){
	case CONFIG_WAIT_H:
	  t = eeprom_data[0].p_wait_cloud % 60; // preserve minutes
	  eeprom_data[0].p_wait_cloud+=60;
	  if(eeprom_data[0].p_wait_cloud>(99*60+59)) eeprom_data[0].p_wait_cloud = t;
	  //configcounter_s = 10*3; // about 3 seconds
	  break;
	case CONFIG_WAIT_M:
	  t = eeprom_data[0].p_wait_cloud / 60; // preserve hours
	  eeprom_data[0].p_wait_cloud++;
	  if((eeprom_data[0].p_wait_cloud / 60)>t) eeprom_data[0].p_wait_cloud = t*60;
	  //configcounter_s = 10*3; // about 3 seconds
	  break;
	case CONFIG_RUN_M:
	  t = eeprom_data[0].p_run % 60; // preserve seconds
	  eeprom_data[0].p_run+=60;
	  if(eeprom_data[0].p_run>(99*60+59)) eeprom_data[0].p_run = t;
	  //configcounter_s = 10*3; // about 3 seconds
	  break;
	case CONFIG_RUN_S:
	  t = eeprom_data[0].p_run / 60; // preserve minutes
	  eeprom_data[0].p_run++;
	  if((eeprom_data[0].p_run / 60)>t) eeprom_data[0].p_run = t*60;
	  //configcounter_s = 10*3; // about 3 seconds
	  break;
  }
}

// Return night, cloud or sun according to LED light sensor readings
DAYPHASE getDayPhase(void){
  if(glob.Vlight<=LEDSENSOR_NIGHT) return DAYPHASE_NIGHT;
  if(glob.Vlight<=LEDSENSOR_CLOUD && glob.dayphase==DAYPHASE_SUN) return DAYPHASE_CLOUD;
  if(glob.Vlight>=LEDSENSOR_CLOUD && glob.dayphase==DAYPHASE_NIGHT) return DAYPHASE_CLOUD;
  if(glob.Vlight>=LEDSENSOR_SUN && glob.dayphase==DAYPHASE_CLOUD) return DAYPHASE_SUN;
  if(glob.Vlight<(LEDSENSOR_SUN-100) && glob.dayphase==DAYPHASE_SUN) return DAYPHASE_CLOUD;  // if sun behind the clouds - turn light on
  return glob.dayphase;
}

uint16_t getWaitValue(void){
  if(glob.dayphase==DAYPHASE_NIGHT) return eeprom_data[0].p_wait_night; 
  if(glob.dayphase==DAYPHASE_SUN) return eeprom_data[0].p_wait_sun; 
  return eeprom_data[0].p_wait_cloud;
}

// if daylight counter is 0 then we can turn Lights on or off according to dayphase
// if sunny, then Lights should be off
//TODO
void LightsOnOff(void){
  bit l_on = 0;
  // first check sunny day only while daylight
  if(daylight){
      if(glob.dayphase==DAYPHASE_CLOUD) l_on = 1;
  }
}

// switch to daylight mode if counter is 0 or there is cloudy or sunny weather longer than 1 minute
//TODO
void setDaylight(void){

  if(glob.daylight_cntr_s==0){
      daylight = !daylight;
  }


  glob.daylight_cntr_s = eeprom_data[0].daylight;
}
