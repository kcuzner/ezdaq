/**
 * Analog to digital converter HAL
 * Optimized for usage as a DAQ
 *
 * Kevin Cuzner
 */
#ifndef _ADC_H_
#define _ADC_H_

#include "arm_cm4.h"

/**
 * Initializes the ADC for continuous sampling on all available channels
 */
void adc_init(void);

/**
 * Gets the current value of the passed adc channel
 * @param channel Channel to read
 * @return Last read ADC value
 */
uint16_t adc_get_value(uint8_t channel);

#endif //_ADC_H_
