
#ifndef MAIN_H_
#define MAIN_H_

#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include <SI_EFM8BB1_Defs.h>
#include <stdint.h>

/***** COMPILATION VARIANTS *****/
//#define DEBUGUART
//#define DEBUG
//#define PREVENTFLASHWRITE

SI_SBIT(PIN_PUMP, SFR_P1, 2); //PUMP mosfet
SI_SBIT(PIN_SERVOPWR, SFR_P1, 4); //Servo power mosfet
SI_SBIT(PIN_PWM0, SFR_P0, 7);
SI_SBIT(PIN_PWM1, SFR_P1, 0);
SI_SBIT(PIN_PWM2, SFR_P1, 1);

//typedef enum {DISPLAY_EC,DISPLAY_CALIB,DISPLAY_CALIBNUMBLINK,DISPLAY_CALIBBLINK,DISPLAY_WAIT} DISPLAY_STATE;
typedef enum {MACHINE_WAIT, MACHINE_RUN, MACHINE_CONFIG} MACHINE_STATE;
typedef enum {CONFIG_WAIT, CONFIG_RUN, CONFIG_WAIT_H, CONFIG_WAIT_M, CONFIG_RUN_M, CONFIG_RUN_S} CONFIG_STATE;

// Structure for accessing 16bit number by 2 8 bit (back and forth)
// u16 and u8[] sharing the same memory space
// Usage:
// U16_U8 ptr;
// ptr.u8[0] = high;
// ptr.u8[1] = low;
// ptr.u16 = 16bit;
typedef union
   {
   unsigned short u16;
   unsigned char u8[2];
   } U16_U8;


typedef struct
{
	//DISPLAY_STATE displaystate; // what is currently on display
	MACHINE_STATE machinestate; // what does the firmware doing at the moment
	CONFIG_STATE configstate; // different states during config mode
	uint8_t p_wait_sub_s; // seconds counter for wait period
	uint16_t p_wait_cntr_m, p_run_cntr_s; // timer counters (m - minutes, s - seconds)
	int16_t screenSaver_s; // (signed) start screen saver mode after some time of inactivity
} glob_t;

typedef struct
{
  uint16_t p_wait; // pump not working (minutes)
  uint16_t p_run;  // pump is working (seconds)
} eeprom_t;


typedef struct
{
  int16_t set_out[2];
//  int16_t set_out1;
//  int16_t set_out2;
  int16_t cur_out[2];
//  int16_t cur_out1;
//  int16_t cur_out2;
} pwmglob_t;

extern bool pwmOut0_update,pwmOut1_update,pwmOut2_update;
extern pwmglob_t pwmglob;

extern eeprom_t xdata eeprom_data[];

extern glob_t glob;

extern bit second_tick; // set to 1 every second (for time countnig operation)
extern uint8_t pcacntr_s; // counter for seconds based on Timer2
extern uint8_t pcacntr_s10; // counter for 10s of seconds (every 100ms) based on Timer2
extern uint8_t configcounter_s; // counter for button timing while config
extern uint8_t configflashcntr; // counter for flashing screen content while in config
#endif /* MAIN_H_ */
