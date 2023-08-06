/* linker script
 * Tom Trebisky  8/4/2023
 */

/* Link it to run in the XIP (execute in place) region.
. = 0x10000000;
 */

SECTIONS
{
    . = 0x90000000;
    .text   : { *(.text*)   }
}
