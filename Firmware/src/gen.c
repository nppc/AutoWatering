//#pragma src
#include "main.h"
#include "gen.h"
#include "SSD1306.h"

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
		switch (glob.machinestate){
			case MACHINE_WAIT:
				if(p_wait_sub_s>0){
					p_wait_sub_s--;
				}else{
					p_wait_sub_s = 60;
					if(p_wait_cntr_m>0)p_wait_cntr_m--;
					retval=1;
				}
				break;
			case MACHINE_RUN:
				if(p_run_cntr_s>0)p_run_cntr_s--;
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
	ssd1306_printTimeM(24,0,tmp / 60;);
	tmp = tmp % 60;
	ssd1306_printTimeS(64,0,tmp);
}

// format: XXh XXm
void show_time_m(uint16_t num){
	int16_t tmp;
	tmp = num;
	ssd1306_printTimeH(24,0,tmp / 60;);
	tmp = tmp % 60;
	ssd1306_printTimeM(59,0,tmp);
}

void scroll_image(void){
	if(scrolldelay==0 && glob.machimestate=MACHINE_RUN){
		// time to scroll
		scrolldelay=2;
	    scroll_down(11, 2);
		ssd1306_printBitmapX(0, 0, 11, 2, scrlbuff);
	}
}