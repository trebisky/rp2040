/* linker script
 * Tom Trebisky  8/4/2023
 */

/* Link it to run in the XIP (execute in place) region.
 * (irrelevant, this is position independent code)
 */

SECTIONS
{
    . = 0x10000000;
    .text   : { *(.text*)   }
}
