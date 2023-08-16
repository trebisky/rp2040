/* linker script
 * Tom Trebisky  8/4/2023
 */

/* Any base address will do, this is PI code */

SECTIONS
{
    . = 0xdeadbeef;
    .text   : { *(.text*)   }
}
