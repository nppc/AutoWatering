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


void bubblesortSsdots(uint8_t dotsnum){
  uint8_t i,j;
  bit sw;
  for(i=0;i<(dotsnum-1);i++){
      sw=0;
      for(j=0;j<(dotsnum-i-1);j++){
          if(ssdots[j].sramaddr>ssdots[j+1].sramaddr){
              uint8_t tmp;
              tmp = ssdots[j].x;
              ssdots[j].x = ssdots[j+1].x;
              ssdots[j+1].x = tmp;
              tmp = ssdots[j].y;
              ssdots[j].y = ssdots[j+1].y;
              ssdots[j+1].y = tmp;
              tmp = ssdots[j].sramaddr;
              ssdots[j].sramaddr = ssdots[j+1].sramaddr;
              ssdots[j+1].sramaddr = tmp;
              sw=1;
          }
      }
      if(!sw) break;
  }
}

void fillSSaverBuffer(void){
  uint16_t i, i1, dotsnum = (glob.p_wait_cntr_m < SSAVERMAXDOTS ? glob.p_wait_cntr_m : SSAVERMAXDOTS); //dots count is equal to the minutes remaining, but not more than 40
  uint8_t x, y;
  //for(i=0;i<(dotsnum);i++){ssdots[i].x=0;ssdots[i].y=0;} // clear array
  memset(ssdots, 0, sizeof(ssdots));
  // generate random coordinates and make sure they will not repeat
  for(i=0;i<dotsnum;i++){
    x = (uint32_t)96 * rand() / RAND_MAX;
    y = (uint32_t)16 * rand() / RAND_MAX;
    // check do we have the same coords?
    for(i1=0;i1<i;i1++){
      if(ssdots[i1].x==x && ssdots[i1].y==y){
        // generate new coords
        x = (uint32_t)96 * rand() / RAND_MAX;
        y = (uint32_t)16 * rand() / RAND_MAX;
        // restart check
        i1=0;
      }
    }
    ssdots[i].x=x; ssdots[i].y=y;
    // and calculate address in sram of oled
    ssdots[i].sramaddr = x + (y>7 ? 96 : 0);
  }
  bubblesortSsdots(dotsnum);
}


void drawSSaverOled(void){
  uint8_t i, cp=0, dotsnum = (glob.p_wait_cntr_m < SSAVERMAXDOTS ? glob.p_wait_cntr_m : SSAVERMAXDOTS);;
  setCol(0);
  setRow(0);
  ssd1306_write_display_start();
  for(i=0;i<(96*2);i++){
	uint8_t tmp=0, d=0;
	if(cp<dotsnum) tmp=ssdots[cp].sramaddr;
	if(i==tmp){
		//we found address with a star. Need to check how many stars we have at the same address
		d = 1 << (ssdots[cp].y-(ssdots[cp].y>7 ? 8 : 0)); // first dot
		while(1){
			cp++;
			if(cp==dotsnum) break; // we reached the end
			if(ssdots[cp].sramaddr == tmp){
				d |= 1 << (ssdots[cp].y-(ssdots[cp].y>7 ? 8 : 0));
			}else{
				break;
			}
		}
	}
    I2C_Write(d);
  }
  I2C_Stop();
}

// remove one star and add another
void replaceSSaverStar(void){
	uint8_t i, x, y, cp, dotsnum = (glob.p_wait_cntr_m < SSAVERMAXDOTS ? glob.p_wait_cntr_m : SSAVERMAXDOTS);
	bit dup=0;
	cp = (uint32_t)dotsnum * rand() / RAND_MAX;
	if(cp==dotsnum){
	    cp=dotsnum-1;
	}
	x = (uint32_t)96 * rand() / RAND_MAX;
	y = (uint32_t)16 * rand() / RAND_MAX;
  if(x==96 || y==16){
      x--;
      y--;
  }

	// make sure it is unique
	for(i=0;i<dotsnum;i++){
		if(x==ssdots[i].x && y==ssdots[i].y) dup=1; // we found duplicate it is fine to leave everything as it is
	}
	// update coordinates if needed
	if(!dup){
	  ssdots[cp].x=x;ssdots[cp].y=y;
		ssdots[cp].sramaddr = x + (y>7 ? 96 : 0);
		bubblesortSsdots(dotsnum);
	}
}
