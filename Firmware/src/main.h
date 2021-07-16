
#ifndef MAIN_H_
#define MAIN_H_

#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include <SI_EFM8BB1_Defs.h>
#include <stdint.h>

/***** COMPILATION VARIANTS *****/
//#define DEBUGUART
//#define DEBUG
#define PREVENTFLASHWRITE

SI_SBIT(PIN_1, SFR_P1, 0); //P+
SI_SBIT(PIN_2, SFR_P1, 1); //P-

typedef enum {DISPLAY_EC,DISPLAY_CALIB,DISPLAY_CALIBNUMBLINK,DISPLAY_CALIBBLINK,DISPLAY_WAIT} DISPLAY_STATE;

typedef struct
{
	DISPLAY_STATE displaystate; // what is currently on display
	uint8_t batcheckcntr;
} glob_t;

typedef struct
{
  uint8_t d;
} eeprom_t;

extern eeprom_t xdata eeprom_data[];

extern glob_t glob;

#endif /* MAIN_H_ */
