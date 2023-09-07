# Makefile for rp2040 second stage boot
#
# Tom Trebisky  9-5-2023
#

SDK = /u1/Projects/rp2040/pico-examples/build/pico-sdk/src/rp2_common/boot_stage2

all:	boot2-sdk.bin

boot2-sdk.bin:
	cp $(SDK)/bs2_default.bin boot2-sdk.bin

boot2.uf2: boot2-sdk.bin
	mkuf2 boot2-sdk.bin boot2.uf2

zap.bin:
	dd bs=240 count=1 if=/dev/zero of=zap.bin

zap.uf2: zap.bin
	mkuf2 zap.bin zap.uf2

# this does zap existing code in flash, but does not return
# to the factory fresh way of acting like mass storage.
zap:	zap.uf2
	cp zap.uf2 /run/media/tom/RPI-RP2

# This won't work
# The idea was to write just the second stage boot and see if
# a previously loaded thing would still work.
# It fails because the flash gets erased in 4K blocks,
# which must be done before a sector can be rewritten.
boot2:	boot2.uf2
	cp boot2.uf2 /run/media/tom/RPI-RP2

clean:
	rm -f *.bin
