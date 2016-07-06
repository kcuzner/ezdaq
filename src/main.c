/**
 * EZDAC
 *
 * A simple Teensy 3.1-based DAQ
 */

#include "usb.h"

int main(void)
{
    //set up LED for an output on PC5
    PORTC_PCR5 = PORT_PCR_MUX(0x1);
    GPIOC_PDDR = 1 << 5;

    adc_init();
    usb_init();
    EnableInterrupts;

    //GPIOC_PSOR = 1 << 5;

    while (1) { }

    return 0;
}
