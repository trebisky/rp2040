	.cpu cortex-m0
	.arch armv6s-m
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 2
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"blink.c"
	.text
	.align	1
	.p2align 2,,3
	.global	blink_delay
	.syntax unified
	.code	16
	.thumb_func
	.type	blink_delay, %function
blink_delay:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #128
	sub	sp, sp, #8
	lsls	r3, r3, #12
	str	r3, [sp, #4]
.L2:
	ldr	r3, [sp, #4]
	subs	r2, r3, #1
	str	r2, [sp, #4]
	cmp	r3, #0
	bne	.L2
	add	sp, sp, #8
	@ sp needed
	bx	lr
	.size	blink_delay, .-blink_delay
	.align	1
	.p2align 2,,3
	.global	blink
	.syntax unified
	.code	16
	.thumb_func
	.type	blink, %function
blink:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #32
	push	{r4, lr}
	ldr	r3, .L14
	ldr	r1, .L14+4
	sub	sp, sp, #8
	str	r2, [r3]
.L6:
	ldr	r3, [r1, #8]
	tst	r2, r3
	beq	.L6
	movs	r1, #5
	movs	r3, #204
	ldr	r2, .L14+8
	movs	r0, #128
	str	r1, [r2, r3]
	movs	r1, #208
	movs	r4, #128
	lsls	r1, r1, #24
	lsls	r0, r0, #18
	str	r0, [r1, #36]
	lsls	r4, r4, #12
.L9:
	str	r0, [r1, #20]
	str	r4, [sp, #4]
.L7:
	ldr	r3, [sp, #4]
	subs	r2, r3, #1
	str	r2, [sp, #4]
	cmp	r3, #0
	bne	.L7
	str	r0, [r1, #24]
	str	r4, [sp]
.L8:
	ldr	r3, [sp]
	subs	r2, r3, #1
	str	r2, [sp]
	cmp	r3, #0
	bne	.L8
	b	.L9
.L15:
	.align	2
.L14:
	.word	1073803264
	.word	1073790976
	.word	1073823744
	.size	blink, .-blink
	.ident	"GCC: (Fedora 12.2.0-3.fc38) 12.2.0"
