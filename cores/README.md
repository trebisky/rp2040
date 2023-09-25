RP2040 - cores

September 24, 2023

The game now is to start the second core in the RP2040 running.
A side goal is to discover at what speed each core is running at.

This demo began by copying the "printf" demo, not that it was in
particular a logical extension thereof, but it is nice to have
a printf function, and this was the last demo I worked on.
This is my general method.  I copy my latest demo and build on it.

We now have a second stage boot (so no limits on code size) and a
fairly decent linker script.  We still don't zero the bss, nor do
we handle initialized data properly.

* Some notes on the uart and printf --

I decided to put the uart on pins 16 and 17.  Most people will expect it
to be on pins 0 and 1, a trivial change in uart.c will make it so.

I use a second pico running the picotool firmware.  This provides access to the
serial port as ttyACM0 at 115200 baud.  I use picocom as follows:

picocom -b 115200 /dev/ttyACM0
