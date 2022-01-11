//#pragma src
//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================

// USER INCLUDES
#include "main.h"
#include "gen.h"
#include "adc.h"
#include "button.h"
#include "SSD1306.h"

SI_INTERRUPT (ADC0EOC_ISR, ADC0EOC_IRQn)
{
  ADC0CN0_ADINT = 0; // reset interrupt flag
  Adc_data[adcglob.samples_count]=ADC0;
  ADC_buffer_ready = 1; // assume buffer is filled
  if(adcglob.samples_count>0){
    adcglob.samples_count--;
    ADC0CN0_ADBUSY = 1; // start NEXT ADC reading
    ADC_buffer_ready = 0; // buffer is not filled yet
  }
}

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
// enters interrupt every 1ms
//-----------------------------------------------------------------------------
// every 1ms timer
SI_INTERRUPT (TIMER2_ISR, TIMER2_IRQn)
  {
    // advance delay timer
    if(delay_on) {
        if(tmp_millis!=0)
          { tmp_millis--;}
        else
          { delay_on=0;}
    }

    // count 10s of second and seconds
    pcacntr_s10--;
    if(pcacntr_s10==0){
      pcacntr_s10 = 100;

      pcacntr_s--;
      if(pcacntr_s==0){
        second_tick = 1;
        pcacntr_s = 10;
      }

      // counter for config logic delays
      if(configcounter_s!=0)configcounter_s--;

      // counter for config data flashing on screen
      if(configflashcntr!=0)configflashcntr--;
    }


#ifdef SCROLLING
  if(scrolldelay>0)scrolldelay--;
#endif

    TMR2CN0_TF2H = 0;
  }
//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// TIMER0 ISR Content goes here. Remember to clear flag bits:
// TCON::TF0 (Timer 0 Overflow Flag)
//
//-----------------------------------------------------------------------------
// every 32ms while button is pressed
SI_INTERRUPT (TIMER0_ISR, TIMER0_IRQn)
  {
    TCON_TF0=0;
    buttoncntr++;
    buttonstate = BUT_PRESSED;
}

//-----------------------------------------------------------------------------
// INT1_ISR
//-----------------------------------------------------------------------------
//
// INT1 ISR Content goes here. Remember to clear flag bits:
// TCON::IE1 (External Interrupt 1)
//
//-----------------------------------------------------------------------------
// Interrupt on button release
SI_INTERRUPT (INT1_ISR, INT1_IRQn)
  {
    if(buttoncntr>161)
      { // about 5 seconds
        buttonstate = BUT_LONGPRESS;
      }
    else if(buttoncntr>3)
      {
        buttonstate = BUT_SHORTPRESS;
      }
    buttoncntr=0;
  }


