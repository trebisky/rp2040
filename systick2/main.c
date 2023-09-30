/* main.c
 *
 * Start a second core
 *
 * Tom Trebisky  9-24-2023
 */

#include "protos.h"

typedef unsigned int u32;
typedef volatile unsigned int vu32;

#ifdef notdef
void
talker ( void )
{
	int x = 0;
	int led_state = 0;

	for ( ;; ) {
	    x++;
	    // uart_putc ( 'H' );
	    // uart_puts ( "Hello\n" );
	    // printf ( "Hello Sailor!\n" );
	    // printf ( "Count: %d %X\n", x, x );
	    printf ( "Hello\n" );
	    printf ( "Name: %s\n", "Sam" );
	    printf ( "Count: %x\n", x );
	    printf ( "Count: %X\n", x );
	    printf ( "Count: %h\n", x );
	    printf ( "Count: %d\n", x );
	    printf ( " char:: %c\n", '5' );
	    io_delay ();
	    if ( led_state ) {
		led_on ();
		led_state = 0;
	    } else {
		led_off ();
		led_state = 1;
	    }
	}
}
#endif

/* There might be a more clever way to handle this.
 * a number of important things are at base address 0xE0000000
 * but they are spaced around with big gaps.
 */
struct cpu2 {
	vu32	cpuid;
	vu32	icsr;
	vu32	vtor;
	vu32	aircr;

	vu32	scr;
	vu32	ccr;
	u32	__pad0;
	vu32	shpr2;

	vu32	shpr3;
	vu32	shcr;
};

// #define CPU_BASE	0xE000E000
#define CPU2_BASE	((struct cpu2 *) 0xE000ED00)

/* The vector table for core 0 */
extern u32 core0_vectors[];

void
set_vtor ( u32 base )
{
	struct cpu2 *cp = CPU2_BASE;

	cp->vtor = base;
}

/* Execution starts here */
void
my_main ( void )
{
	set_vtor ( (u32) core0_vectors );

	led_init ();
	uart_init ();

	printf ( "\n" );
	printf ( " ----- Restart -----\n" );

	pll_show ();
	core_start ();

	systick_init ();

	// blinker ();
	// talker ();

	printf ( "Core 0 finished\n" );
	printf ( "Core 0 spinning ...\n" );
}

/* THE END */
