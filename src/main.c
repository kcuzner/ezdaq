/**
 * EZDAC
 *
 * A simple Teensy 3.1-based DAQ
 */

#include "usb.h"

int main(void)
{
    usb_init();
    EnableInterrupts;

    while (1) { }

    return 0;
}
