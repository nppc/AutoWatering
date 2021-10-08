
#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"

uint8_t getButtonState(void);

typedef enum {BUT_NOTPRESSED,BUT_PRESSED,BUT_PRESSED2S,BUT_PRESSED5S,BUT_SHORTPRESS,BUT_LONGPRESS} BUT_STATE;

extern int16_t buttoncntr;
extern BUT_STATE buttonstate;

#endif /* BUTTON_H_ */
