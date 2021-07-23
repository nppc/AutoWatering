
#ifndef MAIN_H_
#define MAIN_H_

#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include <SI_EFM8BB1_Defs.h>
#include <stdint.h>

/***** COMPILATION VARIANTS *****/
//#define DEBUGUART
//#define DEBUG
#define PREVENTFLASHWRITE

SI_SBIT(PIN_MOSFET, SFR_P0, 3); //Mosfet

typedef enum {DISPLAY_EC,DISPLAY_CALIB,DISPLAY_CALIBNUMBLINK,DISPLAY_CALIBBLINK,DISPLAY_WAIT} DISPLAY_STATE;
typedef enum {MACHINE_WAIT, MACHINE_RUN, MACHINE_CONFIG} MACHINE_STATE;
typedef enum {CONFIG_WAIT, CONFIG_RUN, CONFIG_WAIT_H, CONFIG_WAIT_M, CONFIG_RUN_M, CONFIG_RUN_S} CONFIG_STATE;

typedef struct
{
	DISPLAY_STATE displaystate; // what is currently on display
	MACHINE_STATE machinestate; // what does the firmware doing at the moment
	CONFIG_STATE configstate; // different states during config mode
	uint8_t p_wait_sub_s; // seconds counter for wait period
	uint16_t p_wait_cntr_m, p_run_cntr_s; // timer counters (m - minutes, s - seconds)
	uint16_t screenSaverCntr; // start screen saver mode after some time of inactivity
} glob_t;

typedef struct
{
  uint16_t p_wait; // pump not working (minutes)
  uint16_t p_run;  // pump is working (seconds)
} eeprom_t;

extern eeprom_t xdata eeprom_data[];

extern glob_t glob;

extern bit second_tick; // set to 1 every second (for time countnig operation)
extern uint8_t pcacntr_s; // counter for seconds based on PCA0
extern uint8_t configcounter_s; // counter for button timing while config
extern uint8_t configflashcntr; // counter for flashing screen content while in config
#endif /* MAIN_H_ */
