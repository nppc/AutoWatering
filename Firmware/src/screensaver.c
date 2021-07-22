//#pragma src
#include "main.h"
#include "screensaver.h"
#include "SSD1306.h"
#include "i2c.h"
#include <stdlib.h> // for rand and qsort
#include <string.h> // for memset

// Show random dots on OLED
#define SSAVERMAXDOTS 40 // maximum dots for a screensaver to show at the same time

volatile ssaverdots_t xdata ssdots[SSAVERMAXDOTS]; // use this buffer for generating random dots on the screen


void initSSaver(void){
  uint16_t seed;
  // use temperature sensor to get initial random number
  // start first adc reading
  ADC0CN0_ADINT = 0; // reset interrupt flag
  ADC0CN0_ADBUSY = 1; // start NEXT ADC readings
  while(ADC0CN0_ADINT==0);
  // start second adc reading
  ADC0CN0_ADINT = 0; // reset interrupt flag
  ADC0CN0_ADBUSY = 1; // start NEXT ADC readings
  while(ADC0CN0_ADINT==0);
  seed = ADC0;
  srand (seed);
}

// for qsort function
int16_t compare (const void * a, const void * b)
{
  ssaverdots_t *oa = (ssaverdots_t *)a;
  ssaverdots_t *ob = (ssaverdots_t *)b;
  return ( (int16_t)ob->sramaddr - (int16_t)oa->sramaddr );
  //return ( *(*)a - *(int*)b );
}

void fillSSaverBuffer(void){
  uint16_t i, i1, dotsnum = (glob.p_wait_cntr_m < SSAVERMAXDOTS ? glob.p_wait_cntr_m : SSAVERMAXDOTS); //dots count is equal to the minutes remaining, but not more than 40
  uint8_t x, y;
  //for(i=0;i<(dotsnum);i++){ssdots[i].x=0;ssdots[i].y=0;} // clear array
  memset(ssdots, 0, sizeof(ssdots));
  // generate random coordinates and make sure they will not repeat
  for(i=0;i<dotsnum;i++){
	x = (uint32_t)95 * rand() / RAND_MAX; 
	y = (uint32_t)15 * rand() / RAND_MAX;
	// check do we have the same coords?
	for(i1=0;i1<i;i1++){
		if(ssdots[i1].x==x && ssdots[i1].y==y){
			// generate new coords
			x = (uint32_t)95 * rand() / RAND_MAX; 
			y = (uint32_t)15 * rand() / RAND_MAX;
			// restart check
			i1=0;
		}
	}
	ssdots[i].x=x; ssdots[i].y=y;
	// and calculate address in sram of oled
	ssdots[i].sramaddr = x + (y>7 ? 96 : 0)
  }
  qsort(ssdots, sizeof(ssaverdots_t), compare);
}


void fillSSaverOled(void){
  uint8_t i;
  setCol(0);
  setRow(0);
  ssd1306_write_display_start();
  for(i=0;i<(96*2);i++){
    I2C_Write(oledbuff[i]);
  }
  I2C_Stop();
}
