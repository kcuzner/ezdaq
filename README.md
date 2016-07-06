# EZDAQ
### Kevin Cuzner

A Teensy 3.1-based DAQ designed for simple data collection.

I found that I needed something to log a long term (several days long) test that
I was performing on an analog circuit of mine. This is a very simple program
that uses the ADC on the Teensy 3.1 to simply gather data continuously on all
topside analog channels (A0-A9). It communicates over USB to some host software
written in Python which performs the conversion from raw ADC output into useful
voltage values. It supports logging all 10 channels at once, a variable time
step between sample records, and per-channel attenuation correction values for
any resistor divider used in front of the analog channels.

This is not an accurate system by any means. Getting it calibrated so that two
channels output the same value for a given voltage may be impossible unless
some seriously precise resistors are used. I am mainly using this to see a trend
in a voltage over time, so it basically functions as a very inaccurate long time
data logging voltmeter for me. Perhaps you have an idea to make this thing more
useful?

## Installation and Usage

1. Clone the repository
2. Run `make` to build (requires `arm-none-eabi` GCC installed)
3. Run `make install` to upload the hex file to an attached Teensy 3.1 (requires
   `teensy_loader_cli` to be on the PATH)
4. Run the `host/ezdaq` python script (requires Python3 and PyUSB)

