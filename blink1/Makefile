# Makefile for the rp2040 blink demo
#
# Tom Trebisky  8-5-2023

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

OBJS = start.o blink.o

all:	blink.uf2 blink.dump blink.ss

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

blink.ss:	blink.c
	$(CC) -S blink.c
	mv blink.s blink.ss

blink.o:	blink.c
	$(CC) -c blink.c

# Interestingly, start.s does not run cpp as I want,
# but when I rename to start.S I can use preprocessor stuff.
start.o : start.S
	#$(AS) start.s -o start.o
	$(AS) -c start.S

gdbtui:
	arm-none-eabi-gdb -tui -q -ex "target extended-remote :3333" --readnow  blink.elf

clean:
	rm -f *.o a.out
	rm -f *.elf *.bin
	#rm -f blink.ss blink.dump
	rm -f blink.s
	rm -f *.uf2


