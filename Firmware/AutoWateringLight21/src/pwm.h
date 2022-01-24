#ifndef PWM_H_
#define PWM_H_

/*
extern void inc_PWMout1(int16_t val);
extern void dec_PWMout1(int16_t val);
extern void inc_PWMout2(int16_t val);
extern void dec_PWMout2(int16_t val);
extern void inc_PWMout0(int16_t val);
extern void dec_PWMout0(int16_t val);
*/
//extern void enable_PWMout0(void);
//extern void enable_PWMout1(void);
//extern void enable_PWMout2(void);
//extern void setval_PWMout0(int16_t val);
//extern void setval_PWMout1(int16_t val);
//extern void setval_PWMout2(int16_t val);
extern void setval_PWMout(int8_t ch, int16_t val);
extern void turnLedsOn(uint16_t spd);
extern void turnLedsOff(uint16_t spd);

//extern void disable_PWMout0(void);
//extern void disable_PWMout1(void);
//extern void disable_PWMout2(void);


#endif /* PWM_H_ */
