@ start.s
@ a tiny bit of assembly code to get going.
@ All we do is set up the stack pointer

.cpu cortex-m0
.thumb

/* We have 256K + 8K of sram
 * 0x2000_0000 to 0x2004_1fff
 */

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

_spin:
    @ spin here if we return
    b .

.global core_vectors
.balign	256
core_vectors:
	.word	0	// reset stack
	.word nada	// reset entry
	.word nada	// NMI
	.word nada	// hard fault

	.word nada	// 4 never happen
	.word nada
	.word nada
	.word nada

	.word nada	// 3 never happen
	.word nada
	.word nada
	.word nada	// svcall

	.word nada	// -- never
	.word nada	// -- never
	.word nada	// pendsv
	.word nada	// systick

	.word nada	// irq0
	.word nada
	.word nada
	.word nada

	.word nada
	.word nada
	.word nada
	.word nada

	.word nada	// irq8
	.word nada
	.word nada
	.word nada

	.word nada
	.word nada
	.word nada
	.word nada

	.word nada	// irq16
	.word nada
	.word nada
	.word nada

	.word nada
	.word nada
	.word nada
	.word nada

	.word nada	// irq24
	.word nada
	.word nada
	.word nada

	.word nada
	.word nada
	.word nada
	.word nada	// irq11

.thumb_func
nada:
	b nada


// The following is a silly useless function,
// but it clues us in to the C/assembly interface.
// The key thing is that the argument is passed in r0
// and the result is returned in r0.
// int digit ( int *val )
// {
//         *val = 0;
//         return 9;
// }
//
// Note also that r2 and r3 are used with impunity
// What I read is that r1, r2, and r3 are scratch registers
// r1 could be a second argument, but given that we don't
// have a second argument it could be a scratch register
// (so why does r7 get used (and saved?)
// r12 could also be used as a scratch register, but has
// some special restrictions I don't fully understand and
// we don't need it, so never mind for now.
//
// 10000230 <digit>:
// 10000230:       b580            push    {r7, lr}
// 10000232:       b082            sub     sp, #8
// 10000234:       af00            add     r7, sp, #0
// 10000236:       6078            str     r0, [r7, #4]
// 10000238:       687b            ldr     r3, [r7, #4]
// 1000023a:       2200            movs    r2, #0
// 1000023c:       601a            str     r2, [r3, #0]
// 1000023e:       2309            movs    r3, #9
// 10000240:       0018            movs    r0, r3
// 10000242:       46bd            mov     sp, r7
// 10000244:       b002            add     sp, #8
// 10000246:       bd80            pop     {r7, pc}

// Let's see if we can recode the above, but in a bit nicer
// and simpler fashion.  Indeed we can --
.global digit1
.thumb_func
digit1:
	movs	r2, #0
	str	r2, [r0]
	movs	r0, #9
	bx	lr

// Now, let's interface to the rp2040 divide hardware.

#define SIO_BASE                    0xD0000000

// Calculate p/q where p is argument and q is 10
.global digit2
.thumb_func
digit2:
	ldr	r1,=SIO_BASE
	ldr	r2, [r0]
	str	r2, [r1,#0x60]
	movs	r2, #10		// divide by 10
	str	r2, [r1,#0x64]

	// Delay for 8 cycles
	// each branch gives us 2 cycles
    b 1f
1:  b 1f
1:  b 1f
1:  b 1f
1:

// Must read quotient last
	ldr	r3, [r1,#0x74]	// remainder
	ldr	r2, [r1,#0x70]	// quotient
	str	r2, [r0]
	movs	r0, r3
	bx	lr

@ THE END
