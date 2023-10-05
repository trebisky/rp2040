/* delay.c
 * delays via CPU intensive counting
 * (rp2040 chip)
 *
 * Tom Trebisky  8-17-2023
 *
 */

#include "protos.h"

typedef unsigned int u32;
typedef volatile unsigned int vu32;

#define	BIT(n)	(1 << (n))

// ==========================

// This blinks at about 1 Hz
#define BLINK_DELAY_COUNT     0x80000

#define TICK_DELAY_COUNT	0x8000

#define TALK_DELAY_COUNT	0x20000
// #define TALK_DELAY_COUNT	0x120000

/* The chip boots up using the crystal (12 Mhz).
 * We later set up the PLL to bump the sys clock to 125 Mhz.
 */
static int cpu_clock = 12;

static int blink_delay_init = BLINK_DELAY_COUNT;
static int tick_delay_init = TICK_DELAY_COUNT;
static int io_delay_init = TALK_DELAY_COUNT;

void
blink_delay ( void )
{
        volatile int delay = blink_delay_init;

        while ( delay-- )
            ;
}

void
tick_delay ( void )
{
	volatile int delay = tick_delay_init;

	while ( delay-- )
	    ;
}

void
io_delay ( void )
{
	volatile int delay = io_delay_init;;

	while ( delay-- )
	    ;
}

/* Crude, and we only support 12 and 125
 */
void
set_cpu_clock ( int rate )
{
	cpu_clock = rate;
	if ( cpu_clock == 125 ) {
	    blink_delay_init = BLINK_DELAY_COUNT * 12;
	    tick_delay_init = TICK_DELAY_COUNT * 12;
	    io_delay_init = TALK_DELAY_COUNT * 12;
	} else {
	    blink_delay_init = BLINK_DELAY_COUNT;
	    tick_delay_init = TICK_DELAY_COUNT;
	    io_delay_init = TALK_DELAY_COUNT;
	}
}

/* THE END */

