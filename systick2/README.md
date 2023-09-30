
RP2040 - systick2

September 29, 2023

This demo was begun by copying the "systick" demo.

The idea now is to get both cores generating (and handling)
systick interrupts.  This requires a vector table for each
core (or some clever way to share a single vector table).
The rest is surprisingly very easy.

We still don't zero the bss, nor do we handle initialized data properly.

* Some notes on the uart and printf --

I decided to put the uart on pins 16 and 17.  Most people will expect it
to be on pins 0 and 1, a trivial change in uart.c will make it so.

I use a second pico running the picotool firmware.  This provides access to the
serial port as ttyACM0 at 115200 baud.  I use picocom as follows:

picocom -b 115200 /dev/ttyACM0
