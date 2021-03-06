#include "main.h"
#include "config.h"
#include "SSD1306.h"
#include "bitmaps.h"
#include "gen.h"
#include "pgm.h"

void menu_config_level_0(void){
  if(glob.configstate == CONFIG_WAIT){
    // blink WAIT CONFIG
    if(configflashcntr==0){
      ssd1306_clear_display();
      configflashcntr = 8; // 800ms
    }else if(configflashcntr<5){
      ssd1306_printBitmap(0, 2, 11, 2, hourglass_bitmap);
      show_time_m(2, eeprom_data[0].p_wait);
      delay_ms(10);
    }
  }else if(glob.configstate == CONFIG_RUN){
    // blink RUN CONFIG
    if(configflashcntr==0){
      ssd1306_clear_display();
      configflashcntr = 8; // 800ms
    }else if(configflashcntr<5){
      ssd1306_printBitmap(0, 2, 11, 2, waterrunning_bitmap);
      show_time_s(2, eeprom_data[0].p_run);
      delay_ms(10);
    }
  }
}

void menu_config_level_1(void){
  switch (glob.configstate){
    case CONFIG_WAIT_H:
      // blink WAIT CONFIG HOURS
      if(configcounter_s==0){
          // switch to minutes edit
          glob.configstate = CONFIG_WAIT_M;
          configflashcntr = 0;
          configcounter_s = 10*3; // about 3 seconds
      }else if(configflashcntr==0){
        ssd1306_printBitmap(0, 2, 11, 2, hourglass_bitmap);
        show_time_m(2, eeprom_data[0].p_wait);
        ssd1306_printBitmapClear(22, 2, 36, 2);
        configflashcntr = 8; // 800ms
      }else if(configflashcntr<5){
        ssd1306_printTimeH(22,2,eeprom_data[0].p_wait / 60);
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
        ssd1306_printBitmap(0, 2, 11, 2, waterrunning_bitmap);
        show_time_s(2, eeprom_data[0].p_run);
        ssd1306_printBitmapClear(22, 2, 41, 2);
        configflashcntr = 8; // 800ms
      }else if(configflashcntr<5){
        ssd1306_printTimeM(22,2,eeprom_data[0].p_run / 60);
        delay_ms(10);
      }
      break;
    case CONFIG_WAIT_M:
      if(configcounter_s==0){
          glob.machinestate = MACHINE_WAIT;
		  glob.configstate = CONFIG_NOCONFIG;
          storeSettingsEE();
		  updateDataOnScreen();
          //RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET; // reboot
          // update timer if needed
          if(eeprom_data[0].p_wait<glob.p_wait_cntr_m) glob.p_wait_cntr_m = eeprom_data[0].p_wait;
      }else if(configflashcntr==0){
        ssd1306_printBitmap(0, 2, 11, 2, hourglass_bitmap);
        show_time_m(2, eeprom_data[0].p_wait);
        ssd1306_printBitmapClear(57, 2, 36, 2);
        configflashcntr = 8; // 800ms
      }else if(configflashcntr<5){
        ssd1306_printTimeM(57,2,eeprom_data[0].p_wait % 60);
        delay_ms(10);
      }
      break;
    case CONFIG_RUN_S:
      if(configcounter_s==0){
		  glob.machinestate = MACHINE_WAIT;
  		  glob.configstate = CONFIG_NOCONFIG;
          storeSettingsEE();
  		  updateDataOnScreen();
          //RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET; // reboot
          // update timer if needed
          if(eeprom_data[0].p_run<glob.p_run_cntr_s) glob.p_run_cntr_s = eeprom_data[0].p_run;
      }else if(configflashcntr==0){
        ssd1306_printBitmap(0, 2, 11, 2, waterrunning_bitmap);
        show_time_s(2, eeprom_data[0].p_run);
        ssd1306_printBitmapClear(62, 2, 31, 2);
        configflashcntr = 8; // 800ms
      }else if(configflashcntr<5){
        ssd1306_printTimeS(62,2,eeprom_data[0].p_run % 60);
        delay_ms(10);
      }
      break;
  }

}

void configAdjustValue(CONFIG_STATE state){
  uint8_t t;
  switch (state){
  case CONFIG_WAIT_H:
    t = eeprom_data[0].p_wait % 60; // preserve minutes
    eeprom_data[0].p_wait+=60;
    if(eeprom_data[0].p_wait>(99*60+59)) eeprom_data[0].p_wait = t;
    //configcounter_s = 10*3; // about 3 seconds
    break;
  case CONFIG_WAIT_M:
    t = eeprom_data[0].p_wait / 60; // preserve hours
    eeprom_data[0].p_wait++;
    if((eeprom_data[0].p_wait / 60)>t) eeprom_data[0].p_wait = t*60;
    //configcounter_s = 10*3; // about 3 seconds
    break;
  case CONFIG_RUN_M:
    t = eeprom_data[0].p_run % 60; // preserve seconds
    eeprom_data[0].p_run+=60;
    if(eeprom_data[0].p_run>(99*60+59)) eeprom_data[0].p_run = t;
    //configcounter_s = 10*3; // about 3 seconds
    break;
  case CONFIG_RUN_S:
    t = eeprom_data[0].p_run / 60; // preserve minutes
    eeprom_data[0].p_run++;
    if((eeprom_data[0].p_run / 60)>t) eeprom_data[0].p_run = t*60;
    //configcounter_s = 10*3; // about 3 seconds
    break;
  }
}
