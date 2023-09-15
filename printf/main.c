/* printf_main.c
 *
 * Tom Trebisky  9-15-2023
 */

#include "protos.h"

// This blinks at about 1 Hz
#define DELAY_COUNT	0x80000

void
blink_delay ( void )
{
	volatile int delay = DELAY_COUNT;

	while ( delay-- )
	    ;
}

/* Execution starts here */
void
my_main ( void )
{
	// blinker ();
	led_init ();

	for ( ;; ) {
	    led_on ();
	    blink_delay ();

	    led_off ();
	    blink_delay ();
	}
}

/* THE END */
