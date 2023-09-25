/* cores.c
 * start the second core on a Pi pico board
 * (rp2040 chip)
 *
 * Tom Trebisky  9-15-2023
 */

#include "protos.h"

typedef unsigned int u32;
typedef volatile unsigned int vu32;

/* Offsets for the four "IO flavors" */
#define FL_RW           0x0000
#define FL_XOR          0x1000
#define FL_SET          0x2000
#define FL_CLR          0x3000

#ifdef notdef
#define RESET_BASE      0x4000c000
#define RESET_BASE_RW   (RESET_BASE + FL_RW)
#define RESET_BASE_XOR  (RESET_BASE + FL_XOR)
#define RESET_BASE_SET  (RESET_BASE + FL_SET)
#define RESET_BASE_CLR  (RESET_BASE + FL_CLR)

struct resets {
    vu32         reset;
    vu32         wdsel;
    vu32         done;
};
#endif

/* SIO
 * Single cycle IO
 * see section 2.3.1 of the datasheet
 */

struct sio {
    vu32		cpuid;			/* 00 */
    vu32		gpio_in;
    vu32		gpio_hi_in;
    u32		_pad0;

    vu32		gpio_out_rw;		/* 10 */
    vu32		gpio_out_set;
    vu32		gpio_out_clr;
    vu32		gpio_out_xor;

    vu32		gpio_oe_rw;		/* 20 */
    vu32		gpio_oe_set;
    vu32		gpio_oe_clr;
    vu32		gpio_oe_xor;

    vu32		gpio_hi_out_rw;		/* 30 */
    vu32		gpio_hi_out_set;
    vu32		gpio_hi_out_clr;
    vu32		gpio_hi_out_xor;

    vu32		gpio_hi_oe_rw;		/* 40 */
    vu32		gpio_hi_oe_set;
    vu32		gpio_hi_oe_clr;
    vu32		gpio_hi_oe_xor;

    vu32		fifo_status;		/* 50 */
    vu32		fifo_wr;
    vu32		fifo_rd;
    vu32		spinlock_st;
};

#define SIO_BASE                    ((struct sio *) 0xD0000000)

#define	FIFO_ROE	8	/* fifo was read when empty */
#define	FIFO_WOF	4	/* fifo was written when full */
#define	FIFO_RDY	2	/* Tx fifo is not full */
#define	FIFO_VLD	1	/* Rx fifo is "valid" (not empty) */

#define SEV()		asm volatile ( "sev\r\n" )

/* =================================================================== */

/* The following is from section 2.8.2 of the datasheet (page 133)
 * Also look at pico-bootrom/bootrom/bootrom_rt0.S
 * which is the bootrom code that is responding to this.
 */

#ifdef notdef
// vector_table is value for VTOR register
// sp is initial stack pointer (SP)
// entry is the initial program counter (PC) (don't forget to set the thumb bit!)
const uint32_t cmd_sequence[] =
{0, 0, 1, (uintptr_t) vector_table, (uintptr_t) sp, (uintptr_t) entry};

uint seq = 0;

do {
    uint cmd = cmd_sequence[seq];
    // always drain the READ FIFO (from core 1) before sending a 0
    if (!cmd) {
	// discard data from read FIFO until empty
	multicore_fifo_drain();
	// execute a SEV as core 1 may be waiting for FIFO space
	__sev();
    }
    // write 32 bit value to write FIFO
    multicore_fifo_push_blocking(cmd);
    // read 32 bit value from read FIFO once available
    uint32_t response = multicore_fifo_pop_blocking();
    // move to next state on correct response (echo-d value) otherwise start over
    seq = cmd == response ? seq + 1 : 0;
} while (seq < count_of(cmd_sequence));
#endif

static void
fifo_flush ( void )
{
	struct sio *sp = SIO_BASE;
	int x;

	while ( sp->fifo_status & FIFO_VLD ) {
	    x = sp->fifo_rd;
	    printf ( "Flush read: %X\n", x );
	}
	printf ( "Fifo status = %X\n", sp->fifo_status );
}

static u32
fifo_read ( void )
{
	struct sio *sp = SIO_BASE;
	int tmo = 99999;

	while ( tmo-- ) {
	    if ( sp->fifo_status & FIFO_VLD )
		return sp->fifo_rd;
	}
	printf ( " -- Fifo read timed out\n" );
	return 0xdead;
}

void
core_entry ( void )
{
	// for ( ;; ) ;
	blinker ();
}

extern u32 core_vectors[];

typedef void (*vfptr) ( void );

void
core_start ( void )
{
	struct sio *sp = SIO_BASE;
	u32 x;
	u32 val;
	vfptr core_addr = core_entry;

	/* After reset, I read 0x3 */
	printf ( "Fifo status = %X\n", sp->fifo_status );

	fifo_flush ();
	sp->fifo_wr = 0;
	SEV ();
	x = fifo_read ();
	printf ( "Send 0, got %X\n", x );

	fifo_flush ();
	sp->fifo_wr = 0;
	SEV ();
	x = fifo_read ();
	printf ( "Send 0, got %X\n", x );

	sp->fifo_wr = 1;
	SEV ();
	x = fifo_read ();
	printf ( "Send 1, got %X\n", x );


	val = (u32) core_vectors;
	sp->fifo_wr = val;
	SEV ();
	x = fifo_read ();
	printf ( "Send V, got %X\n", x );

	val = 0x20004000;
	sp->fifo_wr = val;
	SEV ();
	x = fifo_read ();
	printf ( "Send S, got %X\n", x );

	// val = (u32) core_entry;
	val = (u32) core_addr;
	sp->fifo_wr = val;
	SEV ();
	x = fifo_read ();
	printf ( "Send E, got %X\n", x );
}

/* THE END */
