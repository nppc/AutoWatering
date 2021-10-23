#ifndef GEN_H_
#define GEN_H_

#include "main.h"

extern uint16_t tmp_millis;
extern bit delay_on;
extern uint8_t scrolldelay; // delay for scrolling

void delay_ms(uint16_t ms_count);
void pinsHighZ(void);
void pinsPushPull(void);
uint16_t div32round(uint32_t d1, uint32_t d2);
bit run_timers(void);
void show_time_s(uint16_t num);
void show_time_m(uint16_t num);

void scroll_image(void);
void updateDataOnScreen(void);
void configAdjustValue(CONFIG_STATE state);
DAYPHASE getDayPhase(void);
uint16_t getWaitValue(void);
void setLightsOnOff(void);
void setDaylight(void)

#endif /* GEN_H_ */
