/* printf_main.c
 *
 * Tom Trebisky  9-15-2023
 */

#include "protos.h"

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
