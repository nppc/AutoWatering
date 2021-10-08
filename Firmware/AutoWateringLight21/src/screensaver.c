//#pragma src
#include "main.h"
#include "screensaver.h"
#include "SSD1306.h"
#include "i2c.h"
#include "adc.h"
#include <stdlib.h> // for rand
#include <string.h> // for memset


volatile ssaverdots_t xdata ssdots[SSAVERMAXDOTS]; // use this buffer for generating random dots on the screen
volatile bit ssaverstart = 0, ssaverupdate=1;

void initSSaver(void){
  uint16_t seed;
  while(!processADC());
  seed = glob.Vlight;
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
              uint16_t tmpa;
              tmp = ssdots[j].x;
              ssdots[j].x = ssdots[j+1].x;
              ssdots[j+1].x = tmp;
              tmp = ssdots[j].y;
              ssdots[j].y = ssdots[j+1].y;
              ssdots[j+1].y = tmp;
              tmpa = ssdots[j].sramaddr;
              ssdots[j].sramaddr = ssdots[j+1].sramaddr;
              ssdots[j+1].sramaddr = tmpa;
              sw=1;
          }
      }
      if(!sw) break;
  }
}

void fillSSaverBuffer(void){
  uint8_t i, i1, dotsnum = (glob.p_wait_cntr_m < SSAVERMAXDOTS ? glob.p_wait_cntr_m : SSAVERMAXDOTS); //dots count is equal to the minutes remaining, but not more than 40
  uint8_t x, y;
  //for(i=0;i<(dotsnum);i++){ssdots[i].x=0;ssdots[i].y=0;} // clear array
  memset(ssdots, 0, sizeof(ssdots));
  // generate random coordinates and make sure they will not repeat
  for(i=0;i<dotsnum;i++){
    x = (uint32_t)SSD1306_COLUMNS * rand() / RAND_MAX;
    y = (uint32_t)(SSD1306_PAGES*8) * rand() / RAND_MAX;
    // check do we have the same coords?
    for(i1=0;i1<i;i1++){
      if(ssdots[i1].x==x && ssdots[i1].y==y){
        // generate new coords
        x = (uint32_t)SSD1306_COLUMNS * rand() / RAND_MAX;
        y = (uint32_t)(SSD1306_PAGES*8) * rand() / RAND_MAX;
        // restart check
        i1=0;
      }
    }
    ssdots[i].x=x; ssdots[i].y=y;
    // and calculate address in sram of oled
    //ssdots[i].sramaddr = x + (y>7 ? SSD1306_COLUMNS : 0);
    ssdots[i].sramaddr = (uint16_t)x + ((uint16_t)y / 8 * SSD1306_COLUMNS);
  }
  bubblesortSsdots(dotsnum);
}


void drawSSaverOled(void){
  uint16_t i;
  uint8_t cp=0, dotsnum = (glob.p_wait_cntr_m < SSAVERMAXDOTS ? glob.p_wait_cntr_m : SSAVERMAXDOTS);;
  setCol(0);
  setRow(0);
  ssd1306_write_display_start();
  for(i=0;i<(SSD1306_COLUMNS*SSD1306_PAGES);i++){
      uint16_t tmp=0;
      uint8_t d=0;
    if(cp<dotsnum) tmp=ssdots[cp].sramaddr;
    if(i==tmp){
      //we found address with a star. Need to check how many stars we have at the same address
      //d = 1 << (ssdots[cp].y-(ssdots[cp].y>7 ? 8 : 0)); // first dot
        d = 1 << (ssdots[cp].y-(ssdots[cp].y / 8 * 8)); // first dot
      while(1){
        cp++;
        if(cp==dotsnum) break; // we reached the end
        if(ssdots[cp].sramaddr == tmp){
          //d |= 1 << (ssdots[cp].y-(ssdots[cp].y>7 ? 8 : 0));
            d |= 1 << (ssdots[cp].y-(ssdots[cp].y / 8 * 8));
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
	x = (uint32_t)SSD1306_COLUMNS * rand() / RAND_MAX;
	y = (uint32_t)(SSD1306_PAGES*8) * rand() / RAND_MAX;
  if(x==SSD1306_COLUMNS || y==(SSD1306_PAGES*8)){
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
		//ssdots[cp].sramaddr = x + (y>7 ? SSD1306_COLUMNS : 0);
    ssdots[cp].sramaddr = (uint16_t)x + ((uint16_t)y / 8 * SSD1306_COLUMNS);
	  bubblesortSsdots(dotsnum);
	}
}
