RP2040 - blink1_asm

August 5, 2023

This is my blink1 demo coded entirely in assembly language

This is sort of a "proof of concept" demo to illustrate the possibilities
of using the RP2040 for learning assembly language.

It is also surprising how much this is condensed (76 bytes instead of 124)
compared to the C version (even with -O2).  And it doesn't use SRAM at all.
