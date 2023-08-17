# Makefile for the rp2040 uart demo
#
# Tom Trebisky  8-5-2023

ARMCPU = -mcpu=cortex-m0 -mthumb

# The -Os option optimizes for size.
# (I saw a reduction from 240 bytes to 116)
# Use -O3 to optimize for speed.
#COPTS = -Wall -ffreestanding $(ARMCPU)
#COPTS = -Wall -O2 -ffreestanding $(ARMCPU)
#COPTS = -Wall -gdwarf-4 -ffreestanding $(ARMCPU)
#COPTS = -Wall -ffreestanding $(ARMCPU)
#COPTS = -Wall -gdwarf-4 -ffreestanding $(ARMCPU)
COPTS = -Wall -Os -ffreestanding $(ARMCPU)

AOPTS = --warn --fatal-warnings $(ARMCPU)
LDOPTS = -nostdlib

CC = arm-none-eabi-gcc $(COPTS)
#AS = arm-none-eabi-as $(AOPTS)
AS = arm-none-eabi-gcc $(COPTS)
LD = arm-none-eabi-ld $(LDOPTS)
OBJ = arm-none-eabi-objcopy -O binary
#DUMP = arm-linux-gnu-objdump -d
DUMP = arm-none-eabi-objdump -d

OBJS = start.o uart.o

all:	uart.uf2 uart.dump

install:	uart.uf2
	cp uart.uf2 /run/media/tom/RPI-RP2

# I did a fair bit of debugging using LED code, but that was too big
# to fit in flash (so we used -s), but once this was debugged I
# could comment that out, and it fit in flash (232 bytes)
uart.uf2:	uart.bin
	mkuf2 uart.bin uart.uf2
	#mkuf2 -s uart.bin uart.uf2

uart.bin:	uart.elf
	$(OBJ) uart.elf uart.bin

uart.dump:	uart.elf
	$(DUMP) uart.elf >uart.dump

uart.elf:	$(OBJS)
	$(LD) -T uart.lds $(OBJS) -o uart.elf

uart.o:	uart.c
	$(CC) -c uart.c

# Interestingly, start.s does not run cpp as I want,
# but when I rename to start.S I can use preprocessor stuff.
start.o : start.S
	#$(AS) start.s -o start.o
	$(AS) -c start.S

gdbtui:
	arm-none-eabi-gdb -tui -q -ex "target extended-remote :3333" --readnow  uart.elf

clean:
	rm -f *.o a.out
	rm -f *.elf *.bin
	rm -f uart.dump
	rm -f *.uf2


