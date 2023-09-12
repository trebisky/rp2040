/* linker script for boot2 (second stage boot)
 * Tom Trebisky  9/11/2023
 *
 * This ought to be position dependent code and
 * so we could use any link address.
 */

SECTIONS
{
    . = 0x20041f00;
    .text   : { *(.text*)   }
}
