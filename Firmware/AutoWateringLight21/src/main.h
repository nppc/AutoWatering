#ifndef MAIN_H_
#define MAIN_H_

#include <SI_EFM8BB2_Register_Enums.h>                  // SFR declarations
#include <SI_EFM8BB2_Defs.h>
#include <stdint.h>

/***** COMPILATION VARIANTS *****/
//#define DEBUGUART
#define DEBUG
#define PREVENTFLASHWRITE

SI_SBIT(SDA_PIN, SFR_P0, 1);
SI_SBIT(SCL_PIN, SFR_P0, 2);
SI_SBIT(PIN_PUMP, SFR_P1, 2); //PUMP mosfet
//SI_SBIT(LIGHTSNSR_PIN, SFR_P1, 3);
SI_SBIT(PIN_SERVOPWR, SFR_P1, 4); //Servo power mosfet
SI_SBIT(PIN_PWM0, SFR_P0, 7);
SI_SBIT(PIN_PWM1, SFR_P1, 0);
SI_SBIT(PIN_PWM2, SFR_P1, 1);

typedef enum {MACHINE_WAIT, MACHINE_RUN, MACHINE_CONFIG} MACHINE_STATE;
typedef enum {CONFIG_NOCONFIG,
              CONFIG_WAIT,
              CONFIG_RUN,
              CONFIG_LIGHT,
              CONFIG_WAIT_H,
              CONFIG_WAIT_M,
              CONFIG_RUN_M,
              CONFIG_RUN_S,
              CONFIG_LIGHT_H,
              CONFIG_LIGHT_M
  } CONFIG_STATE;
typedef enum {DAYPHASE_NIGHT, DAYPHASE_CLOUD, DAYPHASE_SUN} DAYPHASE;

#define ADCRESOLUTIONMAXVALUE 4092
#define ADC_SAMPLES 64
#define LED_ADC_SAMPLES 8 // ADC samples for calculating average
#define TMPR_SAMPLES 8  // ADC samples for calculating average

#define TEMPERATUREMAX 60 //60 // if higher - turn off led light
#define TEMPERATUREADJUST 2 // Manual adjust due to inaccuracy of the sensor

// Values for sun/cloud/night LED sensor readings
#define LEDSENSOR_SUN 770 // ADC value
#define LEDSENSOR_CLOUD 720 // ADC value
#define LEDSENSOR_NIGHT 150 // ADC value
#define LEDSENSOR_MORNING 120 // ADC value

#define LIGHTPANELPWM_MAX 930 //957 10 bit PWM value
#define LIGHTPANELPWM_MIN 2 // 10 bit PWM value
#define LIGHTPANELSPEED 5	// Adjust Light Panel PWM every N ms.

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


// globals as struct
typedef enum {V_LIGHT,TMP_BRD,CH0_ADC} ADC_ch;
#define ADC_CH_COUNT 3

typedef struct
{
	//DISPLAY_STATE displaystate; // what is currently on display
	MACHINE_STATE machinestate; // what does the firmware doing at the moment
	CONFIG_STATE configstate; // different states during config mode
	uint8_t p_wait_sub_s; // seconds counter for wait period
	uint16_t p_wait_cntr_m, p_run_cntr_s; // timer counters (m - minutes, s - seconds)
	uint16_t daylight_cntr_s; // timer counter for daylight (max 18h in seconds) and also a night
	uint8_t dayphase_cntr_s; // if dayphase changed, counter starts to count from 0 to 200.
	int16_t screenSaver_s; // (signed) start screen saver mode after some time of inactivity
  int16_t Vlight, TmpBrd;
  DAYPHASE dayphase; // phase of the day
#ifdef DEBUGUART
  uint16_t cntr_uart;
#endif
} glob_t;

typedef struct
{
  uint16_t p_wait_night; // pump not working (minutes)
  uint16_t p_wait_cloud; // pump not working (minutes)
  uint16_t p_wait_sun; // pump not working (minutes)
  uint16_t p_run;  // pump is working (seconds)
  uint16_t daylight; // daylight interval (18h max in seconds)
  uint16_t night; // night interval calculated from daylight (4h min in seconds)
} eeprom_t;


typedef struct
{
  int16_t set_out[3];
  int16_t cur_out[3];
  uint8_t pwmchangecntr;
} pwmglob_t;

extern bit pwmOut0_update,pwmOut1_update,pwmOut2_update; //,pwmchangecntr;
extern bit startup, daylight; // daylight or night mode
extern pwmglob_t pwmglob;

extern eeprom_t xdata eeprom_data[];

extern glob_t glob;

extern bit second_tick; // set to 1 every second (for time countnig operation)
extern uint8_t pcacntr_s; // counter for seconds based on Timer2
extern uint8_t pcacntr_s10; // counter for 10s of seconds (every 100ms) based on Timer2
extern uint8_t configcounter_s; // counter for button timing while config
extern uint8_t configflashcntr; // counter for flashing screen content while in config
#endif /* MAIN_H_ */
