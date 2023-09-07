/* linker script
 * Tom Trebisky  8/4/2023
 */

/* Link it to run in the XIP (execute in place) region.
 * offset by 0x100 to leave room for second stage boot
 */

SECTIONS
{
    . = 0x10000100;
    .text   : { *(.text*)   }
}
