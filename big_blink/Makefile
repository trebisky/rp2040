# Makefile for the rp2040 BIG blink demo
#
# Tom Trebisky  9-5-2023

ARMCPU = -mcpu=cortex-m0 -mthumb

#COPTS = -Wall -O2 -ffreestanding $(ARMCPU)
COPTS = -Wall -gdwarf-4 -ffreestanding $(ARMCPU)
#COPTS = -Wall -ffreestanding $(ARMCPU)
AOPTS = --warn --fatal-warnings $(ARMCPU)
LDOPTS = -nostdlib

CC = arm-none-eabi-gcc $(COPTS)
#AS = arm-none-eabi-as $(AOPTS)
AS = arm-none-eabi-gcc $(COPTS)
LD = arm-none-eabi-ld $(LDOPTS)
OBJ = arm-none-eabi-objcopy -O binary
#DUMP = arm-linux-gnu-objdump -d
DUMP = arm-none-eabi-objdump -d

#BOOT2 = ../boot2/boot2-sdk.bin
#BOOT2 = ../boot2/boot2_vec.bin
BOOT2 = ../boot2/boot2_novec.bin

OBJS = start.o blink.o

all:	blink.uf2

install:	blink.uf2
	cp blink.uf2 /run/media/tom/RPI-RP2

blink.uf2:	blink.bin
	mkuf2 -b $(BOOT2) blink.bin blink.uf2

blink.bin:	blink.elf
	$(OBJ) blink.elf blink.bin

blink.dump:	blink.elf
	$(DUMP) blink.elf >blink.dump

blink.elf:	$(OBJS)
	$(LD) -T blink.lds $(OBJS) -o blink.elf

blink.o:	blink.c
	$(CC) -c blink.c

# Interestingly, start.s does not run cpp as I want,
# but when I rename to start.S I can use preprocessor stuff.
start.o : start.S
	#$(AS) start.s -o start.o
	$(AS) -c start.S

SDK = /u1/Projects/rp2040/pico-examples/build/blink

# This is 512*39 bytes in size -- 19968 bytes
sdkblink.uf2:
	cp $(SDK)/blink.uf2 sdkblink.uf2

sdkblink.bin:
	cp $(SDK)/blink.bin sdkblink.bin

clean:
	rm -f *.o a.out
	rm -f *.elf *.bin
	rm -f blink.dump
	rm -f *.uf2

# THE END
