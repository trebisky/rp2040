RP2040 - mkuf2

August 17, 2023

This is a host side utility to generate a uf2 file from a binary image.

mkuf2 [-s] xyz.bin xyz.uf2

The -s switch tells it to put it into SRAM, the default is to flash.

This will complain if flash is selected and the image is bigger than 252 bytes.
