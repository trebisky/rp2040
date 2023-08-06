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

all:	makeuf2

# You may need to be root to do this.
install:	makeuf2
	cp makeuf2 /usr/local/bin

makeuf2 : makeuf2.c crcpico.h
	gcc -O2 makeuf2.c -o makeuf2

clean:
	rm -f makeuf2
