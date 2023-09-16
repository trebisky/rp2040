/* printf_main.c
 *
 * Tom Trebisky  9-15-2023
 */

#include "protos.h"

void
talker ( void )
{
	int x = 0;

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
	    printf ( " char:: %c\n", '5' );
	    io_delay ();
	}
}

/* Execution starts here */
void
my_main ( void )
{
	led_init ();
	uart_init ();

	// blinker ();
	talker ();
}

/* THE END */
