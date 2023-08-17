	.cpu cortex-m0
	.arch armv6s-m
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"blink.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "blink.c"
	.align	1
	.global	blink_delay
	.syntax unified
	.code	16
	.thumb_func
	.type	blink_delay, %function
blink_delay:
.LFB0:
	.loc 1 69 1
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #8
	.cfi_def_cfa_offset 16
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	.loc 1 70 15
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r7, #4]
	.loc 1 72 8
	nop
.L2:
	.loc 1 72 15 discriminator 1
	ldr	r3, [r7, #4]
	subs	r2, r3, #1
	str	r2, [r7, #4]
	.loc 1 72 10 discriminator 1
	cmp	r3, #0
	bne	.L2
	.loc 1 74 1
	nop
	nop
	mov	sp, r7
	add	sp, sp, #8
	@ sp needed
	pop	{r7, pc}
	.cfi_endproc
.LFE0:
	.size	blink_delay, .-blink_delay
	.align	1
	.global	do_reset
	.syntax unified
	.code	16
	.thumb_func
	.type	do_reset, %function
do_reset:
.LFB1:
	.loc 1 78 1
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #16
	.cfi_def_cfa_offset 24
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	str	r0, [r7, #4]
	.loc 1 81 12
	ldr	r3, .L5
	str	r3, [r7, #12]
	.loc 1 82 19
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #12]
	str	r2, [r3]
	.loc 1 84 12
	ldr	r3, .L5+4
	str	r3, [r7, #12]
	.loc 1 85 15
	nop
.L4:
	.loc 1 85 20 discriminator 1
	ldr	r3, [r7, #12]
	ldr	r3, [r3, #8]
	.loc 1 85 27 discriminator 1
	ldr	r2, [r7, #4]
	ands	r3, r2
	.loc 1 85 34 discriminator 1
	beq	.L4
	.loc 1 87 1
	nop
	nop
	mov	sp, r7
	add	sp, sp, #16
	@ sp needed
	pop	{r7, pc}
.L6:
	.align	2
.L5:
	.word	1073803264
	.word	1073790976
	.cfi_endproc
.LFE1:
	.size	do_reset, .-do_reset
	.align	1
	.syntax unified
	.code	16
	.thumb_func
	.type	set_io_func, %function
set_io_func:
.LFB2:
	.loc 1 91 1
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #16
	.cfi_def_cfa_offset 24
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	str	r0, [r7, #4]
	str	r1, [r7]
	.loc 1 94 13
	ldr	r3, .L8
	str	r3, [r7, #12]
	.loc 1 95 28
	ldr	r2, [r7]
	ldr	r1, [r7, #12]
	ldr	r3, [r7, #4]
	lsls	r3, r3, #3
	adds	r3, r1, r3
	adds	r3, r3, #4
	str	r2, [r3]
	.loc 1 96 1
	nop
	mov	sp, r7
	add	sp, sp, #16
	@ sp needed
	pop	{r7, pc}
.L9:
	.align	2
.L8:
	.word	1073823744
	.cfi_endproc
.LFE2:
	.size	set_io_func, .-set_io_func
	.align	1
	.global	blink
	.syntax unified
	.code	16
	.thumb_func
	.type	blink, %function
blink:
.LFB3:
	.loc 1 101 1
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #8
	.cfi_def_cfa_offset 16
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	.loc 1 105 2
	movs	r0, #32
	bl	do_reset
	.loc 1 107 12
	movs	r3, #208
	lsls	r3, r3, #24
	str	r3, [r7, #4]
	.loc 1 109 2
	movs	r1, #5
	movs	r0, #25
	bl	set_io_func
	.loc 1 116 18
	ldr	r3, [r7, #4]
	movs	r2, #128
	lsls	r2, r2, #18
	str	r2, [r3, #36]
.L11:
	.loc 1 122 23 discriminator 1
	ldr	r3, [r7, #4]
	movs	r2, #128
	lsls	r2, r2, #18
	str	r2, [r3, #20]
	.loc 1 123 6 discriminator 1
	bl	blink_delay
	.loc 1 125 23 discriminator 1
	ldr	r3, [r7, #4]
	movs	r2, #128
	lsls	r2, r2, #18
	str	r2, [r3, #24]
	.loc 1 126 6 discriminator 1
	bl	blink_delay
	.loc 1 122 23 discriminator 1
	nop
	b	.L11
	.cfi_endproc
.LFE3:
	.size	blink, .-blink
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x239
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF23
	.byte	0xc
	.4byte	.LASF24
	.4byte	.LASF25
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.ascii	"u32\000"
	.byte	0x1
	.byte	0xa
	.byte	0x16
	.4byte	0x31
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF19
	.uleb128 0x4
	.4byte	0x31
	.uleb128 0x5
	.4byte	.LASF0
	.byte	0x1
	.byte	0xb
	.byte	0x1f
	.4byte	0x38
	.uleb128 0x6
	.4byte	.LASF6
	.byte	0xc
	.byte	0x1
	.byte	0x19
	.byte	0x8
	.4byte	0x7e
	.uleb128 0x7
	.4byte	.LASF1
	.byte	0x1
	.byte	0x1a
	.byte	0x12
	.4byte	0x3d
	.byte	0
	.uleb128 0x7
	.4byte	.LASF2
	.byte	0x1
	.byte	0x1b
	.byte	0x12
	.4byte	0x3d
	.byte	0x4
	.uleb128 0x7
	.4byte	.LASF3
	.byte	0x1
	.byte	0x1c
	.byte	0x12
	.4byte	0x3d
	.byte	0x8
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.byte	0x1
	.byte	0x26
	.byte	0x2
	.4byte	0xa2
	.uleb128 0x7
	.4byte	.LASF4
	.byte	0x1
	.byte	0x27
	.byte	0xb
	.4byte	0x3d
	.byte	0
	.uleb128 0x7
	.4byte	.LASF5
	.byte	0x1
	.byte	0x28
	.byte	0xc
	.4byte	0x3d
	.byte	0x4
	.byte	0
	.uleb128 0x6
	.4byte	.LASF7
	.byte	0xf0
	.byte	0x1
	.byte	0x25
	.byte	0x8
	.4byte	0xbc
	.uleb128 0x9
	.ascii	"io\000"
	.byte	0x1
	.byte	0x29
	.byte	0x4
	.4byte	0xbc
	.byte	0
	.byte	0
	.uleb128 0xa
	.4byte	0x7e
	.4byte	0xcc
	.uleb128 0xb
	.4byte	0x31
	.byte	0x1d
	.byte	0
	.uleb128 0xc
	.ascii	"sio\000"
	.byte	0x30
	.byte	0x1
	.byte	0x2e
	.byte	0x8
	.4byte	0x14f
	.uleb128 0x7
	.4byte	.LASF8
	.byte	0x1
	.byte	0x2f
	.byte	0xa
	.4byte	0x14f
	.byte	0
	.uleb128 0x7
	.4byte	.LASF9
	.byte	0x1
	.byte	0x30
	.byte	0xb
	.4byte	0x3d
	.byte	0x10
	.uleb128 0x7
	.4byte	.LASF10
	.byte	0x1
	.byte	0x31
	.byte	0xb
	.4byte	0x3d
	.byte	0x14
	.uleb128 0x7
	.4byte	.LASF11
	.byte	0x1
	.byte	0x32
	.byte	0xb
	.4byte	0x3d
	.byte	0x18
	.uleb128 0x7
	.4byte	.LASF12
	.byte	0x1
	.byte	0x33
	.byte	0xb
	.4byte	0x3d
	.byte	0x1c
	.uleb128 0x7
	.4byte	.LASF13
	.byte	0x1
	.byte	0x35
	.byte	0xb
	.4byte	0x3d
	.byte	0x20
	.uleb128 0x7
	.4byte	.LASF14
	.byte	0x1
	.byte	0x36
	.byte	0xb
	.4byte	0x3d
	.byte	0x24
	.uleb128 0x7
	.4byte	.LASF15
	.byte	0x1
	.byte	0x37
	.byte	0xb
	.4byte	0x3d
	.byte	0x28
	.uleb128 0x7
	.4byte	.LASF16
	.byte	0x1
	.byte	0x38
	.byte	0xb
	.4byte	0x3d
	.byte	0x2c
	.byte	0
	.uleb128 0xa
	.4byte	0x25
	.4byte	0x15f
	.uleb128 0xb
	.4byte	0x31
	.byte	0x3
	.byte	0
	.uleb128 0xd
	.4byte	.LASF20
	.byte	0x1
	.byte	0x64
	.byte	0x1
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x184
	.uleb128 0xe
	.ascii	"sp\000"
	.byte	0x1
	.byte	0x66
	.byte	0xe
	.4byte	0x184
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0xf
	.byte	0x4
	.4byte	0xcc
	.uleb128 0x10
	.4byte	.LASF26
	.byte	0x1
	.byte	0x5a
	.byte	0x1
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1ce
	.uleb128 0x11
	.4byte	.LASF17
	.byte	0x1
	.byte	0x5a
	.byte	0x13
	.4byte	0x1ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x11
	.4byte	.LASF18
	.byte	0x1
	.byte	0x5a
	.byte	0x1d
	.4byte	0x1ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xe
	.ascii	"iop\000"
	.byte	0x1
	.byte	0x5c
	.byte	0x1a
	.4byte	0x1da
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x12
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x4
	.4byte	0x1ce
	.uleb128 0xf
	.byte	0x4
	.4byte	0xa2
	.uleb128 0x13
	.4byte	.LASF21
	.byte	0x1
	.byte	0x4d
	.byte	0x1
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x214
	.uleb128 0x14
	.ascii	"who\000"
	.byte	0x1
	.byte	0x4d
	.byte	0x10
	.4byte	0x1ce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0xe
	.ascii	"rp\000"
	.byte	0x1
	.byte	0x4f
	.byte	0x18
	.4byte	0x214
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0xf
	.byte	0x4
	.4byte	0x49
	.uleb128 0x15
	.4byte	.LASF27
	.byte	0x1
	.byte	0x44
	.byte	0x1
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x16
	.4byte	.LASF22
	.byte	0x1
	.byte	0x46
	.byte	0xf
	.4byte	0x1d5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF24:
	.ascii	"blink.c\000"
.LASF19:
	.ascii	"unsigned int\000"
.LASF13:
	.ascii	"gpio_oe_rw\000"
.LASF12:
	.ascii	"gpio_out_xor\000"
.LASF5:
	.ascii	"ctrl\000"
.LASF18:
	.ascii	"func\000"
.LASF7:
	.ascii	"io_bank0\000"
.LASF10:
	.ascii	"gpio_out_set\000"
.LASF20:
	.ascii	"blink\000"
.LASF4:
	.ascii	"status\000"
.LASF26:
	.ascii	"set_io_func\000"
.LASF23:
	.ascii	"GNU C17 12.2.0 -mcpu=cortex-m0 -mthumb -mfloat-abi="
	.ascii	"soft -march=armv6s-m -gdwarf-4 -ffreestanding\000"
.LASF2:
	.ascii	"wdsel\000"
.LASF11:
	.ascii	"gpio_out_clr\000"
.LASF3:
	.ascii	"done\000"
.LASF9:
	.ascii	"gpio_out_rw\000"
.LASF16:
	.ascii	"gpio_oe_xor\000"
.LASF22:
	.ascii	"delay\000"
.LASF25:
	.ascii	"/u1/Projects/rp2040/Archive/blink1\000"
.LASF14:
	.ascii	"gpio_oe_set\000"
.LASF27:
	.ascii	"blink_delay\000"
.LASF1:
	.ascii	"reset\000"
.LASF8:
	.ascii	"_pad\000"
.LASF6:
	.ascii	"resets\000"
.LASF15:
	.ascii	"gpio_oe_clr\000"
.LASF0:
	.ascii	"vu32\000"
.LASF21:
	.ascii	"do_reset\000"
.LASF17:
	.ascii	"gpio\000"
	.ident	"GCC: (Fedora 12.2.0-3.fc38) 12.2.0"
