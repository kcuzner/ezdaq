/**
 * Analog to digital converter HAL
 * Optimized for usage as a DAQ
 *
 * Kevin Cuzner
 */

#include "adc.h"

#include <stdint.h>

#define N_CHANNELS 10

typedef struct {
    const uint8_t channel;
    const uint8_t useBChannel;
    uint16_t value;
} ADCChannel;

static uint8_t conversion_channel;
static ADCChannel channels[N_CHANNELS] = {
    { .channel = 5, .useBChannel = 1 }, //A0
    { .channel = 14 }, //A1
    { .channel = 8 }, //A2
    { .channel = 9 }, //A3
    { .channel = 13 }, //A4
    { .channel = 12 }, //A5
    { .channel = 6, .useBChannel = 1 }, //A6
    { .channel = 7, .useBChannel = 1 }, //A7
    { .channel = 15 }, //A8
    { .channel = 4, .useBChannel = 1 } //A9
};

static void adc_begin_conversion(uint8_t channel)
{
    //if they request an invalid channel, reset the channel to 0
    if (channel >= N_CHANNELS)
        channel = 0;

    //set up the channel for this conversion
    conversion_channel = channel;

    //Set up CFG2
    ADC0_CFG2 = ADC_CFG2_ADACKEN_MASK | (channels[channel].useBChannel ? ADC_CFG2_MUXSEL_MASK : 0);

    //Set up SC1A for a conversion
    //Note that this initiates the conversion, so it should be done after all
    //setup.
    ADC0_SC1A = ADC_SC1_AIEN_MASK | (channels[channel].channel & ADC_SC1_ADCH_MASK);
}

void adc_init(void)
{
    uint16_t temp;

    //Enable ADC0 module
    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;

    //Set up conversion precision and clock speed for calibration
    ADC0_CFG1 = ADC_CFG1_MODE(0x1) | ADC_CFG1_ADIV(0x1) | ADC_CFG1_ADICLK(0x3); //12 bit conversion, adc async clock, div by 2 (<3MHz)
    ADC0_CFG2 = ADC_CFG2_ADACKEN_MASK; //enable async clock

    //Enable hardware averaging and set up for calibration
    ADC0_SC3 = ADC_SC3_CAL_MASK | ADC_SC3_AVGS(0x3);
    while (ADC0_SC3 & ADC_SC3_CAL_MASK) { }
    if (ADC0_SC3 & ADC_SC3_CALF_MASK) //calibration failed. Quit now while we're ahead.
        return;
    temp = ADC0_CLP0 + ADC0_CLP1 + ADC0_CLP2 + ADC0_CLP3 + ADC0_CLP4 + ADC0_CLPS;
    temp /= 2;
    temp |= 0x1000;
    ADC0_PG = temp;
    temp = ADC0_CLM0 + ADC0_CLM1 + ADC0_CLM2 + ADC0_CLM3 + ADC0_CLM4 + ADC0_CLMS;
    temp /= 2;
    temp |= 0x1000;
    ADC0_PG = temp;

    //Set clock speed for measurements (no division)
    ADC0_CFG1 = ADC_CFG1_MODE(0x1) | ADC_CFG1_ADICLK(0x3); //12 bit conversion, adc async clock, no divide

    //Set up interrupts
    enable_irq(IRQ(INT_ADC0));

    //Set up all ports for their analog functions

    //Begin the first conversion
    adc_begin_conversion(0);
}


uint16_t adc_get_value(uint8_t channel)
{
    if (channel >= N_CHANNELS)
        return 0;

    return channels[channel].value;
}

void ADC0_IRQHandler()
{
    GPIOC_PTOR = 1 << 5;
    channels[conversion_channel].value = ADC0_RA;

    adc_begin_conversion((conversion_channel + 1) % N_CHANNELS);
}

