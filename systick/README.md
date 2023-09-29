
RP2040 - systick

September 25, 2023

This demo was begun by copying the "cores" demo.
For no particular reason, I am going to do my experimenting
with systick from core 1 rather than core 0.
May as well wear some rubber off the tires on core 1.

The name here is a bit misleading.  I did indeed play with systick,
but the main business was discovering that the CPU was running at
12 Mhz coming out of the bootrom (the raw crystal oscillator) and
then figuring out how to get the PLL going to bump it up to 125 Mhz.

Note that both cores are run from the same clock source.
It is not possible to run them at different speeds.
A separate PLL is provided for USB, which must be configured
to run at 48 Mhz, I have not yet played with that.

A main goal was to discover at what speed each core was running at.
We used systick (along with a stopwatch) to sort that out.
Then we continued on from there and did the PLL setup work.

We still don't zero the bss, nor do we handle initialized data properly.

We do actually set up for systick interrupts, and this is all but
trivial.  Add an entry to the vector table, write a short handler,
and set the bit in the systick CSR to enable interrupts.

Systick interrupts are entirely independent of the NVIC.
The are controlled by PRIMASK.

* Some notes on the uart and printf --

I decided to put the uart on pins 16 and 17.  Most people will expect it
to be on pins 0 and 1, a trivial change in uart.c will make it so.

I use a second pico running the picotool firmware.  This provides access to the
serial port as ttyACM0 at 115200 baud.  I use picocom as follows:

picocom -b 115200 /dev/ttyACM0
