RP2040 - uart1

August 17, 2023

Another simple demo, just to send endless characters via the uart.

My uart is on pins 16,17 -- but it is trivial to get it on 0,1.

I keep this small (it is 232 bytes) so it will fit into a single 256 byte
sector, which the rp2040 will load and run thinking it is a second stage
boot loader.  Note that the -Os compiler switch is critical in getting
this size small enough.

Bigger things for another day.
