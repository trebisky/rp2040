@ start.s
@ a tiny bit of assembly code to get going.
@ All we do is set up the stack pointer

.cpu cortex-m0
.thumb

#define STACK	0x20001000

// We put these here when first experimenting and needed to
// work with the boot2 file from the SDK
#ifdef VECTOR
    .word STACK
    .word _start
#endif

// It is vital to declare this a .thumb_func
// This causes an odd address to be generated in
// the "table" above, which is critical.
.global _start
.thumb_func
_start:
    ldr r0,=STACK
    mov sp,r0
    bl my_main

    @ spin here if we return
    b .

@ THE END
