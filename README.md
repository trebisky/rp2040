RP2040

August 5, 2023

Here you have various experiments and projects involving the Pi Pico and
the RP2040 chip.

I was never a great fan of the Raspberry Pi, but the Pico and RP2040 are
another thing entirely.  The more I work with them, the more I like them.
Everything is open source, even the bootrom code.

* uf2 - tool to create and dump uf2 files
* blink1 - simple C program to blink the onboard LED
* blink1_asm - same thing coded in assembly language
* uart1 - simple C program to send characters

I am avoiding the SDK, not that I have anything against it, but for me it takes
all the fun out of playing with the chip.  And I want to learn how it works at
the lowest level.  Here is a list of some things I might do next:

The blink via assembly language was intended to demonstrate that the rp2040
could be used as a platform to learn assembly language programming.

* interrupts
* timer to blink LED with interrupts
* play with the PIO
* learn to put bigger projects into flash
* learn USB from the bottom up
