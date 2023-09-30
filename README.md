RP2040

August 5, 2023

Here you have various "bare metal" experiments and projects
involving the Pi Pico and the RP2040 chip.

I was never a great fan of the Raspberry Pi, but the Pico and RP2040 are
another thing entirely.  The more I work with them, the more I like them.
Everything is open source, even the bootrom code.

First, here are some projects that do not rely on a second stage boot.

* uf2 - tools to create and dump uf2 files
* blink1 - simple C program to blink the onboard LED
* blink1_asm - same thing coded in assembly language
* uart1 - simple C program to send characters

The blink via assembly language demo was intended to demonstrate that the rp2040
could be used as a platform to learn assembly language programming.

I am doing my best to avoid the SDK,
not that I have anything against it, but for me it takes
all the fun out of playing with the chip.
I want to learn how the chip  works at the lowest level.

After the first few experiments, which masqueraded as the second stage boot,
I spent some time learning how the second stage boot works and gathering
the source code to build it myself.

* boot2 - second stage boot
* big_blink - blink1 modified to work with the SSB

Now that I have a handle on how the SSB (second stage boot) works,
I have a lot more elbow room for more interesting projects.

* printf - get a simple printf to work
* cores - get the second core running
* systick - get CPU running at full speed
* systick2 - get systick interrupts from both cores

Things to do in the future

* interrupts
* timer to blink LED with interrupts
* play with the PIO
* learn USB from the bottom up
