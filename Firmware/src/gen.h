#ifndef GEN_H_
#define GEN_H_

#include "main.h"

extern uint16_t tmp_millis;
extern bit delay_on;

void delay_ms(uint16_t ms_count);
void pinsHighZ(void);
void pinsPushPull(void);
uint16_t div32round(uint32_t d1, uint32_t d2);

#endif /* GEN_H_ */
