//#pragma src
// Button logic
// short press - on/off pump
// 2sec press - on/off light
// long press (5sec) - Config/debug info.

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "InitDevice.h"
#include "gen.h"
#include "i2c.h"
#include "SSD1306.h"
#include "button.h"
#include "pgm.h"
#include "bitmaps.h"
#include "screensaver.h"
#include "pwm.h"
#include "adc.h"
#include "config.h"

volatile glob_t glob;
volatile eeprom_t xdata eeprom_data[1];

volatile bit pwmOut0_update,pwmOut1_update,pwmOut2_update; //,pwmchangecntr;
volatile bit needLight = false; // last stae id off. If true, then we need leds to be on. If false, then leds should be off
volatile pwmglob_t pwmglob;

volatile bit second_tick, ms_tick;
volatile uint8_t pcacntr_s = 10;
volatile uint8_t pcacntr_s10 = 100;
volatile uint8_t configcounter_s;
volatile uint8_t configflashcntr;

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup(void) {
	// $[SiLabs Startup]
	// [SiLabs Startup]$
}

void updateDataOnScreen(void){
  // dayphase in the right upper corner
  ssd1306_printBitmap(109, 0, 19, 2, dayphase_bitmap[glob.dayphase]);
  // Light time
  if(needLight) ssd1306_printBitmap(0, 0, 11, 2, lightOn_bitmap); else ssd1306_printBitmap(0, 0, 11, 2, lightOff_bitmap);
  show_time_m(0,glob.ledOnOff_cntr_s/60);
  // watering time
  switch (glob.machinestate){
    case MACHINE_WAIT:
      ssd1306_printBitmap(0, 2, 11, 2, water_bitmap);
      show_time_m(2,glob.p_wait_cntr_m);
#ifdef DEBUG
      //ssd1306_printNumberDebug(64,2,glob.Vlight);
#endif
      break;
    case MACHINE_RUN:
      show_time_s(2,glob.p_run_cntr_s);
      break;
  }

#ifdef DEBUG
  setCol(92); // debug
  setRow(2); // debug
  ssd1306_printSmallLine("P"); // debug
  ssd1306_printSmallNumber(pwmglob.cur_out[0]); // debug
  setCol(92); // debug
  setRow(3); // debug
  ssd1306_printSmallLine("L"); // debug
  ssd1306_printSmallNumber(pwmglob.lightpanelspeed); // debug
  //ssd1306_printSmallNumber(pwmglob.cur_out[0]);
#endif
}

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main(void) {
  // Call hardware initialization routine

  enter_DefaultMode_from_RESET();
  
  pwmglob.lightpanelspeed = LIGHTPANELSPEEDSLOW;

  setval_PWMout(0,LIGHTPANELPWM_MIN);
  setval_PWMout(1,LIGHTPANELPWM_MIN);
  setval_PWMout(2,LIGHTPANELPWM_MIN);

  buttonstate = BUT_NOTPRESSED;
  glob.machinestate = MACHINE_WAIT;
  glob.p_wait_sub_s = 60;
  glob.screenSaver_s = SSAVERDELAY; //SSAVERDELAY seconds of inactivity activates screen saver
  glob.dayphase = DAYPHASE_NOSUN; // Set initial dayphase.
  glob.dayphase_cntr_s = 250; // update variables after startup
  glob.ledOnOff_cntr_s = 0; // We start with expired counter to be able to turn lghts on right after start.

#ifdef DEBUGUART
	prnUART("START",1);
#endif

#ifdef USEKALMAN
	kalmanInit();
#endif

  delay_ms(50); // need for ssd1306 init

  initADC();
  initSSaver();

  loadSettingsEE();


  glob.p_wait_cntr_m = eeprom_data[0].p_wait;
  glob.p_run_cntr_s = eeprom_data[0].p_run;

  //glob.p_wait_cntr_m = 20; // debug
  fillSSaverBuffer();

  ssd1306_init();
  ssd1306_clear_display();
  ssd1306_send_command(SSD1306_DISPLAYON);

  // initialize screen content
  updateDataOnScreen();
  //ssd1306_printBitmap(0, 0, 11, 2, hourglass_bitmap);
  //show_time_m(glob.p_wait_cntr_m);

#ifdef SCROLLING
  scroll_init(11, 2, waterrunning_bitmap); // initialize once, as we scroll only one image
#endif


	while(1){
	  uint8_t but;

#ifndef DEBUG
	  processADC();
#else
	  processADC();
/*
	  if(processADC()){
	      ssd1306_printNumberDebug(0,2,pwmglob.set_out[0]);
	      //ssd1306_printNumberDebug(0,2,glob.TmpBrd);
        //ssd1306_printNumberDebug(64,2,glob.Vlight);
	  }
*/
#endif

	  //glob.dayphase = getDayPhase(); // Get day phase according to LED sensor
	  //setDaylight(); // set variable daylight
	  //setLigthPanelSpeed();
	  setLightsOnOff(); // turn on or off LED lights

    if(run_timers() && glob.screenSaver_s>0){ // process timers
      updateDataOnScreen();
    }
    if(glob.screenSaver_s>0) scroll_image(); // scroll icon when running pump
	
    // show/update screensaver
    if(glob.screenSaver_s==0 && ssaverstart){
      fillSSaverBuffer(); // prepare buffer
      drawSSaverOled();
      ssaverstart = 0;
    }
    if(glob.screenSaver_s<0){
      //update once in a 10 seconds
      if(ssaverupdate){
        replaceSSaverStar();
        drawSSaverOled();
        ssaverupdate=0;
      }
      if(glob.screenSaver_s < -10){
        ssaverupdate=1;
        glob.screenSaver_s = -1;
      }
    }
		
    but = getButtonState();
    // check button
  if(glob.screenSaver_s<=0 && but==BUT_PRESSED){
    // wake up from screen saver on button short press
    ssd1306_clear_display();
    updateDataOnScreen();
    delay_ms(1000);
    buttonstate = BUT_NOTPRESSED;but = BUT_NOTPRESSED;
    ssaverstart = 1; // prepare flag for next time screen saver will be activated
    glob.screenSaver_s = SSAVERDELAY;
  }
  if(but!=BUT_NOTPRESSED){
      glob.screenSaver_s = SSAVERDELAY;
  }
	
    if(but==BUT_PRESSED){ // button is pressed
        configcounter_s = 10*3; // about 3 seconds
    }else if(but==BUT_PRESSED5S){ // button is pressed for longer than 5 seconds and not released
        //long press
        if(glob.machinestate!=MACHINE_CONFIG){
            glob.machinestate = MACHINE_CONFIG;
            glob.configstate = CONFIG_WAIT; // start config wait time
            configcounter_s = 10*3; // about 3 seconds
        }else if(glob.configstate == CONFIG_WAIT && configcounter_s==0){
            glob.configstate = CONFIG_RUN;
            configcounter_s = 10*3; // about 3 seconds
        }else if(glob.configstate == CONFIG_RUN && configcounter_s==0){
            glob.configstate = CONFIG_WAIT;
            configcounter_s = 10*3; // about 3 seconds
        }
    }else if(but==BUT_PRESSED2S){
		// button is pressed for 2 seconds or longer
		// use this only in changing config values
	  if(glob.configstate==CONFIG_WAIT_H || glob.configstate==CONFIG_WAIT_M || glob.configstate==CONFIG_RUN_M || glob.configstate==CONFIG_RUN_S){
			uint16_t tmp;
      IE_EA = 0;     //Disable interrupts
      tmp = buttoncntr;
      IE_EA = 1;      //Enable interrupts
			if(tmp>(21*2)){ // make changing numbers a bit slower
		      IE_EA = 0;     //Disable interrupts
		      tmp = buttoncntr;
		      buttoncntr = 20*2; // about 1.5S
		      IE_EA = 1;      //Enable interrupts
        configflashcntr = 4; // no flash during changing numbers
        configcounter_s = 10*3; // about 3 seconds
        configAdjustValue(glob.configstate);
			}
		}
	}else if(but==BUT_LONGPRESS){
      // button is released from long press
      // Go to subconfig menu
      if(glob.configstate==CONFIG_WAIT){
        glob.configstate = CONFIG_WAIT_H;
        configflashcntr = 0;
        configcounter_s = 10*3; // about 3 seconds
      }else if(glob.configstate==CONFIG_RUN){
        glob.configstate = CONFIG_RUN_M;
        configflashcntr = 0;
        configcounter_s = 10*3; // about 3 seconds
      }
    }else if(but==BUT_SHORTPRESS){
        // turn on/off leds(day/night)
        glob.ledOnOff_cntr_s=0;
      // start/stop pump if not in config
	  switch (glob.machinestate){
        case MACHINE_WAIT:
          glob.p_wait_cntr_m=0;
          break;
        case MACHINE_RUN:
          glob.p_run_cntr_s=0;
          break;
      }
      // process change config values short presses
      configcounter_s = 10*3; // about 3 seconds
      configAdjustValue(glob.configstate);
    }

		// change machine state and update the screen
		switch (glob.machinestate){
      case MACHINE_WAIT:
        if(glob.p_wait_cntr_m==0){
            delay_ms(500);
            PIN_PUMP = 1; //start pump
            glob.machinestate = MACHINE_RUN;
            glob.p_run_cntr_s = eeprom_data[0].p_run;
            ssd1306_clear_display();
            glob.screenSaver_s = SSAVERDELAY;
            show_time_s(2, glob.p_run_cntr_s);
        }
        break;
      case MACHINE_RUN:
        if(glob.p_run_cntr_s==0){
            PIN_PUMP = 0; //stop pump
            delay_ms(200);
            glob.machinestate = MACHINE_WAIT;
            glob.p_wait_cntr_m = getWaitValue();
            glob.p_wait_sub_s = 60;
            ssd1306_clear_display();
            glob.screenSaver_s = SSAVERDELAY;
            ssd1306_printBitmap(0, 2, 11, 2, hourglass_bitmap);
            show_time_m(2, glob.p_wait_cntr_m);
        }
        break;
      case MACHINE_CONFIG:
        menu_config_level_0();
        menu_config_level_1();
        break;
		}
	}

}
