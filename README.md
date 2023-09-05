RP2040

August 5, 2023

Here you have various experiments and projects involving the Pi Pico and
the RP2040 chip.

I was never a great fan of the Raspberry Pi, but the Pico and RP2040 are
another thing entirely.  The more I work with them, the more I like them.
Everything is open source, even the bootrom code.

* uf2 - tools to create and dump uf2 files
* blink1 - simple C program to blink the onboard LED
* blink1_asm - same thing coded in assembly language
* uart1 - simple C program to send characters

The blink via assembly language demo was intended to demonstrate that the rp2040
could be used as a platform to learn assembly language programming.

I am avoiding the SDK, not that I have anything against it, but for me it takes
all the fun out of playing with the chip.  And I want to learn how it works at
the lowest level.  Here is a list of some things I might do next:

I did take time to study the SDK and learn how the second stage boot works,
which led to this:

* boot2 - second stage boot

Things to do in the future

* learn to put bigger projects into flash
* interrupts
* timer to blink LED with interrupts
* second core
* play with the PIO
* learn USB from the bottom up
