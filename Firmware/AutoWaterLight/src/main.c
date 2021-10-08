//#pragma src
//=========================================================
// src/EFM8BB31F32I-C-QFP32_main.c: generated by Hardware Configurator
//
// This file will be updated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================

// Button logic
// short press - fix/release values
// long press (5sec) - calibration. It will cycle from 0EC til 5EC. Release button when desired calibration is found. 
//  Then insert the probe into calibration liquid and wait until it will say, "OK".

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

volatile glob_t glob;
volatile eeprom_t xdata eeprom_data[1];

volatile bool pwmOut0_update,pwmOut1_update,pwmOut2_update;
volatile pwmglob_t pwmglob;

volatile bit second_tick;
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
  switch (glob.machinestate){
    case MACHINE_WAIT:
      ssd1306_printBitmap(0, 0, 11, 2, hourglass_bitmap);
      show_time_m(glob.p_wait_cntr_m);
      break;
    case MACHINE_RUN:
      show_time_s(glob.p_run_cntr_s);
      break;
  }
}

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main(void) {
  // Call hardware initialization routine

  enter_DefaultMode_from_RESET();

  enable_PWMout0();
  enable_PWMout1();
  enable_PWMout2();
  setval_PWMout0(123);
  setval_PWMout1(145);
  setval_PWMout2(120);
//  setval_PWMout(0,123);
//  setval_PWMout(1,145);
//  setval_PWMout(2,120);
  disable_PWMout0();
  disable_PWMout1();
  disable_PWMout2();

  buttonstate = BUT_NOTPRESSED;
  //glob.displaystate = DISPLAY_EC;
  glob.machinestate = MACHINE_WAIT;
  glob.p_wait_sub_s = 60;
  glob.screenSaver_s = SSAVERDELAY; //SSAVERDELAY seconds of inactivity activates screen saver

#ifdef DEBUGUART
	prnUART("START",1);
#endif

  delay_ms(50); // need for ssd1306 init

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
  ssd1306_printBitmap(0, 0, 11, 2, hourglass_bitmap);
  show_time_m(glob.p_wait_cntr_m);

 #ifdef SCROLLING 
  scroll_init(11, 2, waterrunning_bitmap); // initialize once, as we scroll only one image
#endif


	while(1){
	    uint8_t but;

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
            show_time_s(glob.p_run_cntr_s);
        }
        break;
      case MACHINE_RUN:
        if(glob.p_run_cntr_s==0){
            PIN_PUMP = 0; //stop pump
            delay_ms(200);
            glob.machinestate = MACHINE_WAIT;
            glob.p_wait_cntr_m = eeprom_data[0].p_wait;
            glob.p_wait_sub_s = 60;
            ssd1306_clear_display();
            glob.screenSaver_s = SSAVERDELAY;
            ssd1306_printBitmap(0, 0, 11, 2, hourglass_bitmap);
            show_time_m(glob.p_wait_cntr_m);
        }
        break;
      case MACHINE_CONFIG:
        if(glob.configstate == CONFIG_WAIT){
          // blink WAIT CONFIG
          if(configflashcntr==0){
            ssd1306_clear_display();
            configflashcntr = 8; // 800ms
          }else if(configflashcntr<5){
            ssd1306_printBitmap(0, 0, 11, 2, hourglass_bitmap);
            show_time_m(eeprom_data[0].p_wait);
            delay_ms(10);
          }
        }else if(glob.configstate == CONFIG_RUN){
          // blink RUN CONFIG
          if(configflashcntr==0){
            ssd1306_clear_display();
            configflashcntr = 8; // 800ms
          }else if(configflashcntr<5){
            ssd1306_printBitmap(0, 0, 11, 2, waterrunning_bitmap);
            show_time_s(eeprom_data[0].p_run);
            delay_ms(10);
          }
        }
        break;
		}

		switch (glob.configstate){
		  case CONFIG_WAIT_H:
        // blink WAIT CONFIG HOURS
		    if(configcounter_s==0){
		        // switch to minutes edit
		        glob.configstate = CONFIG_WAIT_M;
		        configflashcntr = 0;
		        configcounter_s = 10*3; // about 3 seconds
		    }else if(configflashcntr==0){
          ssd1306_printBitmap(0, 0, 11, 2, hourglass_bitmap);
          show_time_m(eeprom_data[0].p_wait);
          ssd1306_printBitmapClear(22, 0, 36, 2);
          configflashcntr = 8; // 800ms
        }else if(configflashcntr<5){
          ssd1306_printTimeH(22,0,eeprom_data[0].p_wait / 60);
          delay_ms(10);
        }
        break;
      case CONFIG_RUN_M:
        if(configcounter_s==0){
            // switch to minutes edit
            glob.configstate = CONFIG_RUN_S;
            configflashcntr = 0;
            configcounter_s = 10*3; // about 3 seconds
        }else if(configflashcntr==0){
          ssd1306_printBitmap(0, 0, 11, 2, waterrunning_bitmap);
          show_time_s(eeprom_data[0].p_run);
          ssd1306_printBitmapClear(22, 0, 41, 2);
          configflashcntr = 8; // 800ms
        }else if(configflashcntr<5){
          ssd1306_printTimeM(22,0,eeprom_data[0].p_run / 60);
          delay_ms(10);
        }
        break;
      case CONFIG_WAIT_M:
        if(configcounter_s==0){
            glob.machinestate = MACHINE_WAIT;
            storeSettingsEE();
            RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET; // reboot
        }else if(configflashcntr==0){
          ssd1306_printBitmap(0, 0, 11, 2, hourglass_bitmap);
          show_time_m(eeprom_data[0].p_wait);
          ssd1306_printBitmapClear(59, 0, 36, 2);
          configflashcntr = 8; // 800ms
        }else if(configflashcntr<5){
          ssd1306_printTimeM(59,0,eeprom_data[0].p_wait % 60);
          delay_ms(10);
        }
        break;
      case CONFIG_RUN_S:
        if(configcounter_s==0){
            storeSettingsEE();
            RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET; // reboot
        }else if(configflashcntr==0){
          ssd1306_printBitmap(0, 0, 11, 2, waterrunning_bitmap);
          show_time_s(eeprom_data[0].p_run);
          ssd1306_printBitmapClear(64, 0, 31, 2);
          configflashcntr = 8; // 800ms
        }else if(configflashcntr<5){
          ssd1306_printTimeS(64,0,eeprom_data[0].p_run % 60);
          delay_ms(10);
        }
        break;
		}

	}

}