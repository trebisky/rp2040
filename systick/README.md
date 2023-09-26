
RP2040 - systick

September 25, 2023

This demo was begun by copying the "cores" demo.
For no particular reason, I am going to do my experimenting
with systick from core 1 rather than core 0.
May as well wear some rubber off the tires on core 1.

A side goal is to discover at what speed each core is running at.
We thought we might do this as part of the cores demo, but
we never did, and systick may well be a tool to check this out.

We still don't zero the bss, nor do we handle initialized data properly.

* Some notes on the uart and printf --

I decided to put the uart on pins 16 and 17.  Most people will expect it
to be on pins 0 and 1, a trivial change in uart.c will make it so.

I use a second pico running the picotool firmware.  This provides access to the
serial port as ttyACM0 at 115200 baud.  I use picocom as follows:

picocom -b 115200 /dev/ttyACM0
