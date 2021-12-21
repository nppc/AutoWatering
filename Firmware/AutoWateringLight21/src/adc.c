//#pragma src
#include "adc.h"
#include "main.h"
#include "gen.h"
#ifdef DEBUGUART
	#include "uart1.h"
#endif

#ifdef USEKALMAN
	#include <math.h>
#endif

volatile int16_t xdata Adc_data[ADC_SAMPLES];
volatile adcglob_t adcglob;
volatile bit ADC_readTemp, ADC_readLight, ADC_buffer_ready; // ADC_buffer_ready - indicates when new ADC readings are ready

#ifdef USEKALMAN
volatile kalman_t klmf;
#endif

void initADC(void){
  int16_t tmp;
	adcglob.samples_count=ADC_SAMPLES;
	ADC0MX = ADC0MX_ADC0MX__ADC0P11; // set first channel
	ADC0CN0_ADINT = 0;	// reset ADC complete interrupt
	ADC0CN0_ADBUSY = 1;	// start ADC conversions
	// wait for 100 conversions
	ADC_buffer_ready = 0;
	for(tmp=0;tmp<500;tmp++){
    while(processADC()==0);
	}
}

int16_t ConvertADC2mV(int32_t tADC){
  // ADCgain = 1
  // ADCresolution = 4092
  // Vref = 2400mV
  // Vout = tADC * Vref / ADCresolution;
  return tADC * 2400 / ADCRESOLUTIONMAXVALUE;
}


int16_t ConvertADC2temp (uint32_t tADC){
  // Offset (Temperature 0 offset) is 751mV - from datasheet
  // Slope is 2.82mV/C - from datasheet
  // ADCresolution = 4092
  // Vref = 2400mV
  #define tSlope 1231 // Slope * ADCresolution / Vref * 256
  #define tVoffset 1280 // Offset * ADCresolution / Vref
  // return only positive temp
  if((uint16_t)tADC>tVoffset){
	  int16_t tmp = (uint32_t)(tADC-tVoffset) * 256 / tSlope;
	  #ifndef CALIBRATETEMPERATURE
	  tmp = tmp + TEMPERATUREADJUST; // adjust temperature according to calibration. While in calibration do not adjust temperature
	  #endif
      return tmp;
  }else{
      return 0;
  }
}

// Read ADC
// average readings as we do not doing it in interrupts
bit processADC(void){
  int16_t prev_temperature,i;
  uint32_t sum = 0;
  bit retval = 0;
  // cycle between ADC channels and calculate values for pwm control
  if(ADC_buffer_ready){
    uint8_t tmpCh = adcglob.CH_current;// backup current channel
    uint16_t adcval;
    ADC_buffer_ready = 0;
    //switch to temperature channel if it's time
    if(ADC_readTemp){
      adcglob.CH_current = TMP_BRD;
      ADC0MX = ADC0MX_ADC0MX__TEMP;
      adcglob.samples_count=TMPR_SAMPLES-1; // count is 0 based (1 means 2 samples)
      ADC_readTemp = 0;
    }else if(ADC_readLight){
      adcglob.CH_current = V_LIGHT;
      ADC0MX = ADC0MX_ADC0MX__ADC0P11; // LED for light measuring
      adcglob.samples_count=LED_ADC_SAMPLES-1; // count is 0 based (1 means 2 samples)
      //delay_ms(70); // this minimum 50ms delay is needed before reading LED voltage to get accurate value.
      ADC_readLight = 0;
    }else{
        adcglob.CH_current = CH0_ADC;
        ADC0MX = ADC0MX_ADC0MX__ADC0P4; // ANA0
        adcglob.samples_count=ADC_SAMPLES-1; // count is 0 based (1 means 2 samples)
    }
    // don't yet start new adc readings as we need to calculate samples in the buffer
    // for ADC channels we need only averaging
    switch (tmpCh) {
      case V_LIGHT: // battery charge control
        // Lets sum...
        for(i=0;i<LED_ADC_SAMPLES;i++){
          sum += (uint16_t)Adc_data[i];
        }
        #ifdef USEKALMAN
		adcval = kalmanFilter((float)sum / (float)LED_ADC_SAMPLES);
		#else 
		adcval = sum / LED_ADC_SAMPLES; // average	
		#endif
		glob.Vlight = adcval;
        break;
      case TMP_BRD: // board temperature control
        // Lets sum...
        for(i=0;i<TMPR_SAMPLES;i++){
          sum += (uint16_t)Adc_data[i];
        }
        adcval = sum / TMPR_SAMPLES; // average
        prev_temperature = glob.TmpBrd;
        glob.TmpBrd = ConvertADC2temp(adcval);
        break;
      case CH0_ADC: // for ANA0 channel
        retval = 1; // adc read cycle completed
        for(i=0;i<ADC_SAMPLES;i++){
          uint16_t val = Adc_data[i];
          sum += (uint32_t)val;
        }
        // average the sum
        adcval = sum / ADC_SAMPLES; // average
        break;
    }
    // Now buffer can be overwritten
    ADC0CN0_ADINT = 0; // reset interrupt flag
    ADC0CN0_ADBUSY = 1; // start NEXT ADC readings
  }
  return retval;
}

#ifdef USEKALMAN
void kalmanInit(void){
	klmf.err_estimate = KALMAN_e_est;
}

float kalmanFilter(float val){
	klmf.gain = klmf.err_estimate / (klmf.err_estimate + KALMAN_e_mea);
	klmf.current_estimate = klmf.last_estimate + klmf.gain * (val - klmf.last_estimate);
	klmf.err_estimate =  (1.0 - klmf.gain) * klmf.err_estimate + fabs(klmf.last_estimate - klmf.current_estimate) * KALMAN_q;
	klmf.last_estimate = klmf.current_estimate;

	return klmf.current_estimate;	
}
#endif