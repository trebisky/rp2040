RP2040 - printf

September 15, 2023

This is my first demo that takes advantage of having a second stage boot and
being unlimited in code size (i.e. limited only by the size of flash).

It also introduces a more substantial "lds" file that places C variables in
sram where they belong and will eventually allow proper handling of initialized
data, read only data, and the bss area (which will need to be zeroed).

In particular though, the intent is to introduce a printf function.
I am taking a simple one from another project, namely the EBAZ project
(which took it from my STM32F411 project).

Note: I decided to put the uart on pins 16 and 17.  Most people will expect it
to be on pins 0 and 1, a trivial change in uart.c will make it so

I use a second pico running the picotool firmware.  This provides access to the
serial port as ttyACM0 at 115200 baud.  I use picocom as follows:

picocom -b 115200 /dev/ttyACM0
