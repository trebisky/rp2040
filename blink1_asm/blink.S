@ blink.S
@ The whole blink demo in assembly
@
@ Tom Trebisky  8-5-2023

/* Offsets for the four "IO flavors" */
#define FL_RW           0x0000
#define FL_XOR          0x1000
#define FL_SET          0x2000
#define FL_CLR          0x3000

#define RESET_BASE      0x4000c000
#define RESET_BASE_RW   (RESET_BASE + FL_RW)
#define RESET_BASE_XOR  (RESET_BASE + FL_XOR)
#define RESET_BASE_SET  (RESET_BASE + FL_SET)
#define RESET_BASE_CLR  (RESET_BASE + FL_CLR)

#define IO_BANK0_BASE               0x40014000
#define IO_BANK0_BASE_RW	(IO_BANK0_BASE + FL_RW)
#define IO_BANK0_BASE_XOR	(IO_BANK0_BASE + FL_XOR)
#define IO_BANK0_BASE_SET	(IO_BANK0_BASE + FL_SET)
#define IO_BANK0_BASE_CLR	(IO_BANK0_BASE + FL_CLR)

#define SIO_BASE                    0xD0000000

#define SIO_OUT_CLR	0x18
#define SIO_OE_SET	0x24
#define SIO_OUT_SET	0x14

/* This is just one of 32 bits */
#define R_IO_BANK0      0x20

#define GPIO_25      0x02000000

/* An odd thing.  The disassmbly (dump) shows "subs", but we must
 * write it here as "sub" or we get errors.  Some gnu as quirk.
 *
 * This fits in only 76 bytes as compared to 124 for the C version
 */

.cpu cortex-m0
.thumb
.text

@ execution starts here.
@ note that we don't need a stack
@ in fact we don't need SRAM at all.
start:

	@ reset IO Bank 0
	ldr	r1,=RESET_BASE_CLR
	mov	r0,#R_IO_BANK0
	str	r0, [r1]

	@ loop/poll until done
	ldr	r1,=RESET_BASE_RW
wait_loop:
	ldr	r2, [r1,#8]
	tst	r0, r2
	beq	wait_loop

	@ Set function select to software IO
	@ for GPIO 25
	ldr	r1,=IO_BANK0_BASE_RW
	mov	r0,#5
	mov	r2, #0xcc
	str	r0,  [r1,r2]

	@ Enable output for GPIO 25
	ldr	r1,=SIO_BASE
	ldr	r0,=GPIO_25
	str	r0, [r1,#SIO_OE_SET]

blink_loop:
	str	r0, [r1,#SIO_OUT_SET]
	bl	blink_delay
	str	r0, [r1,#SIO_OUT_CLR]
	bl	blink_delay
	b	blink_loop

@ ==================================
@ This blinks at about 1 Hz
#define DELAY_COUNT	0x80000

blink_delay:
	ldr	r3,=DELAY_COUNT
delay_loop:
	sub	r3, r3, #1
	cmp	r3, #0
	bne	delay_loop
	bx	lr

@ THE END
