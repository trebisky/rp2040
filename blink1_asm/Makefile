# Makefile for the rp2040 blink demo
#
# Tom Trebisky  8-5-2023

ARMCPU = -mcpu=cortex-m0 -mthumb

COPTS = -Wall -O2 -ffreestanding $(ARMCPU)
AOPTS = --warn --fatal-warnings $(ARMCPU)
LDOPTS = -nostdlib

CC = arm-none-eabi-gcc $(COPTS)
#AS = arm-none-eabi-as $(AOPTS)
AS = arm-none-eabi-gcc $(COPTS)
LD = arm-none-eabi-ld $(LDOPTS)
OBJ = arm-none-eabi-objcopy -O binary
#DUMP = arm-linux-gnu-objdump -d
DUMP = arm-none-eabi-objdump -d

OBJS = blink.o

all:	blink.uf2 blink.dump

install:	blink.uf2
	cp blink.uf2 /run/media/tom/RPI-RP2

blink.uf2:	blink.bin
	mkuf2 blink.bin blink.uf2

blink.bin:	blink.elf
	$(OBJ) blink.elf blink.bin

blink.dump:	blink.elf
	$(DUMP) blink.elf >blink.dump

blink.elf:	$(OBJS)
	$(LD) -T blink.lds $(OBJS) -o blink.elf

# Interestingly, start.s does not run cpp as I want,
# but when I rename to start.S I can use preprocessor stuff.
blink.o : blink.S
	$(AS) -c blink.S

# =================================
# target remote localhost:3333
gdb:
	arm-none-eabi-gdb blink.elf

clean:
	rm -f *.o a.out
	rm -f *.elf *.bin
	rm -f blink.ss blink.dump
	rm -f blink.s
	rm -f *.uf2
