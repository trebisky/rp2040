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

# ====================================

OBJS = start.o main.o uart.o led.o

all:	main.uf2

install:	main.uf2
	cp main.uf2 /run/media/tom/RPI-RP2

main.uf2:	main.bin
	mkuf2 -b $(BOOT2) main.bin main.uf2

main.bin:	main.elf
	$(OBJ) main.elf main.bin

main.dump:	main.elf
	$(DUMP) main.elf >main.dump

main.elf:	$(OBJS)
	$(LD) -T pico.lds $(OBJS) -o main.elf

main.o:	main.c
	$(CC) -c main.c

# Interestingly, start.s does not run cpp as I want,
# but when I rename to start.S I can use preprocessor stuff.
start.o : start.S
	#$(AS) start.s -o start.o
	$(AS) -c start.S

clean:
	rm -f *.o a.out
	rm -f *.elf *.bin
	rm -f blink.dump
	rm -f *.uf2

# THE END
