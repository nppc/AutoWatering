//#pragma src
#include "main.h"
#include "screensaver.h"
#include "SSD1306.h"
#include "i2c.h"
#include <stdlib.h> // for rand

// Show random dots on OLED

volatile uint8_t xdata oledbuff[96*2]; // use this buffer for generating random dots on the screen

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

void fillSSaverBuffer(void){
  uint16_t i, dotsnum = (glob.p_wait_cntr_m < 40 ? glob.p_wait_cntr_m : 40); //dots count is equal to the minutes remaining, but not more than 40
  uint8_t x, y;
  for(i=0;i<(96*2);i++){oledbuff[i]=0;} // clear array
  // generate random coordinates and make sure they will not repeat
  for(i=0;i<dotsnum;i++){
      while(1){
        x = (uint32_t)(95*2) * rand() / RAND_MAX; // determine byte on a screen
        y = 1 << ((uint32_t)7 * rand() / RAND_MAX); // determine bit in a byte
        // check do we have the same number?
        if(oledbuff[x]!=y) break; //exit while - number is unique
      }
      oledbuff[x]|=y;
  }
  // now buffer prefilled with random dots.
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
