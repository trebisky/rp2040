@ start.s
@ a tiny bit of assembly code to get going.
@ All we do is set up the stack pointer

.cpu cortex-m0
.thumb

#define STACK	0x20001000

    ldr r0,=STACK
    mov sp,r0
    bl blink

    @ spin here if we return
    b .

@ THE END
