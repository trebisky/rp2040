# Makefile for some uf2 file utilities
#
# I began with the makeuf2 utility written by David Welch
#  and took off from there.  See:
#
#  https://github.com/dwelch67/raspberrypi-pico
#
# This is used for RP2040 projects
#
# Tom Trebisky  8-6-2023, 9-4-2023

all:	mkuf2 dump_uf2

# You may need to be root to do this.
install:	mkuf2 dump_uf2
	cp mkuf2 /usr/local/bin
	cp dump_uf2 /usr/local/bin

OBJ = mkuf2.o crc32.o

mkuf2 : $(OBJ)
	gcc $(OBJ) -o mkuf2

mkuf2.o : mkuf2.c
	gcc -O2 -c mkuf2.c

crc32.o : crc32.c
	gcc -O2 -c crc32.c

dump_uf2: dump_uf2.c
	gcc -o dump_uf2 dump_uf2.c

blink.uf2:
	cp /u1/Projects/rp2040/pico-examples/build/blink/blink.uf2 .

test:
	./mkuf2 -s test.bin test.uf2

dump:
	./dump_uf2 blink.uf2

clean:
	rm -f mkuf2
