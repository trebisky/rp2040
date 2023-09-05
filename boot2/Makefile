# Makefile for rp2040 second stage boot
#
# Tom Trebisky  9-5-2023
#

SDK = /u1/Projects/rp2040/pico-examples/build/pico-sdk/src/rp2_common/boot_stage2

all:	boot2-sdk.bin

boot2-sdk.bin:
	cp $(SDK)/bs2_default.bin boot2-sdk.bin

clean:
	rm -f *.bin
