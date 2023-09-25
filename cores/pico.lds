/* linker script
 * Tom Trebisky  8/4/2023
 */

MEMORY
{
   flash(RX)  : ORIGIN = 0x10000100, LENGTH = 2048K
   sram(WAIL) : ORIGIN = 0x20000000, LENGTH = 264K
}

/* Link it to run in the XIP (execute in place) region.
 * offset by 0x100 to leave room for second stage boot
 */

SECTIONS
{
   /* Make sure the vectors are first */
   .text :
   {
        __text_start = .;
       *(.vectors*)
       *(.text*)
       . = ALIGN(4);
       __text_end = .;
   } > flash

   .bss  :
   {
       . = ALIGN(4);
       __bss_start = .;
       *(.bss*)
       *(COMMON)
       . = ALIGN(4);
       __bss_end = .;
   } > sram

   .data :
   {
       . = ALIGN(4);
       __data_start = .;
       *(.data*)
       . = ALIGN(4);
       __data_end = .;
       __end = .;
   } > sram AT> flash

   .rodata :
   {
       . = ALIGN(4);
       __rodata_start = .;
       *(.rodata*)
       . = ALIGN(4);
       __rodata_end = .;
   } > flash
}

/* THE END */
