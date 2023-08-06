/* linker script
 * Tom Trebisky  8/4/2023
 */

/* Link it to run in the XIP (execute in place) region */

SECTIONS
{
    . = 0x10000000;
    .text   : { *(.text*)   }
}
