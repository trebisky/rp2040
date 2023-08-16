# Makefile for the makeuf2 utility
#
# This was written by David Welch and is provided by him
#  with his RP2040 projects.  See:
#
#  https://github.com/dwelch67/raspberrypi-pico
#
# This is used for RP2040 projects
#
# Tom Trebisky  8-6-2023

all:	mkuf2

# You may need to be root to do this.
install:	mkuf2
	cp mkuf2 /usr/local/bin

mkuf2 : mkuf2.c
	gcc -O2 mkuf2.c -o mkuf2

clean:
	rm -f mkuf2
