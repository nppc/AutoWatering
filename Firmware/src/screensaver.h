#ifndef SCREENSAVER_H_
#define SCREENSAVER_H_

typedef struct
{
  uint8_t x,y,sramaddr; // x,y - coordinate of a dot. samebyte - indicates that this coordinate will be on the same OLED SRAM address.
} ssaverdots_t;

extern ssaverdots_t xdata ssdots[];
void initSSaver(void);
void fillSSaverBuffer(void);
void drawSSaverOled(void);
void replaceSSaverStar(void);

#endif /* SCREENSAVER_H_ */
