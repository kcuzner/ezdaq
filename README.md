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

## Installation and Host Usage

1. Clone the repository
2. Run `make` to build (requires `arm-none-eabi` GCC installed)
3. Run `make install` to upload the hex file to an attached Teensy 3.1 (requires
   `teensy_loader_cli` to be on the PATH)
4. Run the `host/ezdaq` python script (requires Python3 and PyUSB)

## Circuit Setup

This uses the pins marked as A0-A9 on the Teensy 3.1 schematic to measure any
voltages present on those pins. All that needs to be done is to place a
capacitor (I used 0.1uF because I literally have 100's of those) and a resistor
divider on that pin. The resistor divider is optional, but I needed it so that I
could measure voltages above 3.3V. Make sure that your resistor divider has a
high enough impedance to minimize the observer effect on any circuit you may
connect this device to. I used 2x 1meg resistors.

To calibrate your resistor dividers, plug them into a known voltage (such as the
3.3V coming off of the Teensy). Then, run the `ezdaq` host program for the
channels you are interested in calibrating. Note the values for each channel and
divide those values by the known voltage. The resulting percentage is your
"attenuation" value that you can plug in using the `-a` argument to `ezdaq`.

For example, if I plugged my resistor divider on channel A0 into 3.3V, I would
run the following command:

```
$ ./ezdaq 0
Time,Channel 0
1467771464.9665403,1.7990478515625
...
```

We now have 1.799 for the "voltage" seen at the pin with an attenuation factor
of 1. If we divide 1.799 by 3.3 we get 0.545 for our attenuation value. Now we
run the following to get our newly calibrated value:

```
$ ./ezdaq -a 0 0.545 0
Time,Channel 0
1467771571.2447994,3.301005232
...
```


## Things to note

 * Unless you are using high-precision resistors, resistor dividers will be
   quite inaccurate. I used two resistors drawn from the same batch and ended up
   with a 45% divider instead of a 50% divider. To make matters worse, my second
   divider from that batch was a 47% divider, so things will vary greatly.
 * I have no clue what I'm doing with the ADC on the Teensy 3.1. It seems
   calibrated enough, but I couldn't quite get 16-bit precision to work over the
   full 0-65535 range. Therefore, I am simply using 12-bit. If someone wants to
   put the time into getting that working, I would appreciate it, though 12 bits
   was sufficient for me.

