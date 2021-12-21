#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"

extern int16_t xdata Adc_data[ADC_SAMPLES]; // ADC samples (0 element is not filled)

typedef struct
	{
    uint8_t samples_count;
    ADC_ch CH_current; // Internal counter for ADC channel switch
    uint8_t temp_read_s; // seconds counter for temperature readings
	} adcglob_t;

extern adcglob_t adcglob;

extern bit ADC_readTemp, ADC_readLight, ADC_buffer_ready;

#ifdef USEKALMAN
#define KALMAN_e_mea 2
#define KALMAN_e_est 2
#define KALMAN_q 0.05

typedef struct
	{
	float err_estimate;
	float current_estimate;
	float last_estimate;
	float gain;
	} kalman_t;

extern kalman_t klmf;
void kalmanInit(void);
float kalmanFilter(float val);
#endif

extern int16_t ConvertADC2mV(int32_t tADC);
extern int16_t ConvertADC2temp (uint32_t tADC);

void initADC(void);
void controlLEDs(void);
bit processADC(void);

#endif /* INC_ADC_H_ */
