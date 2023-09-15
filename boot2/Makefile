# Makefile for rp2040 second stage boot
#
# Tom Trebisky  9-5-2023
#

SDKBIN = /u1/Projects/rp2040/pico-examples/build/pico-sdk/src/rp2_common/boot_stage2
SDKSRC = /u1/Projects/rp2040/pico-sdk/src

ARMCPU = -mcpu=cortex-m0 -mthumb
OPTS = -Wall -ffreestanding $(ARMCPU)
#COPTS = -Wall -gdwarf-4 -ffreestanding $(ARMCPU)
LDOPTS = -nostdlib

AS = arm-none-eabi-gcc $(COPTS)
DUMP = arm-none-eabi-objdump -d
OBJ = arm-none-eabi-objcopy -O binary
LD = arm-none-eabi-ld $(LDOPTS)

# ====================================

#all:	boot2-sdk.bin
all:	boot2_vec.bin boot2_novec.bin

boot2_vec.o: boot2.S
	$(AS) -c boot2.S -o boot2_vec.o

boot2_vec.elf:      boot2_vec.o
	 $(LD) -T boot2.lds boot2_vec.o -o boot2_vec.elf

boot2_vec.bin:      boot2_vec.elf
	$(OBJ) boot2_vec.elf boot2_vec.bin

boot2_vec.dump:     boot2_vec.elf
	$(DUMP) boot2_vec.elf >boot2_vec.dump

boot2_novec.o: boot2.S
	$(AS) -DNOVEC -c boot2.S -o boot2_novec.o

boot2_novec.elf:      boot2_novec.o
	 $(LD) -T boot2.lds boot2_novec.o -o boot2_novec.elf

boot2_novec.bin:      boot2_novec.elf
	$(OBJ) boot2_novec.elf boot2_novec.bin

inc:
#	cp $(SDKSRC)/rp2_common/boot_stage2/boot2_at25sf128a.S boot2.S
#	cp $(SDKSRC)/rp2040/hardware_regs/include/hardware/regs/ssi.h .
#	cp $(SDKSRC)/rp2040/hardware_regs/include/hardware/regs/pads_qspi.h .
#	cp $(SDKSRC)/rp2040/hardware_regs/include/hardware/regs/m0plus.h .

clean:
	rm -f boot2.bin
	rm -f zap.*
	rm -f *.o
	rm -f *.dump *.elf

# ============= some old experiments below here:

boot2-sdk.bin:
	cp $(SDKBIN)/bs2_default.bin boot2-sdk.bin

# Not at all useful, but I wanted to see
# it wrapped into a uf2
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

# THE END
