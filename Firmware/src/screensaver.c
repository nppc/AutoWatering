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
    ssdots[i].sramaddr = x + (y>7 ? 96 : 0);
  }
  bubblesortSsdots(dotsnum);
}


void fillSSaverOled(void){
  uint8_t i;
  setCol(0);
  setRow(0);
  ssd1306_write_display_start();
  for(i=0;i<(96*2);i++){
    //I2C_Write(oledbuff[i]);
  }
  I2C_Stop();
}
