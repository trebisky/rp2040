/* protos.h
 */

void led_init ( void );
void led_on ( void );
void led_off ( void );
void blinker ( void );

void do_reset ( int );
void io_delay ( void );

void uart_init ( void );
void uart_puts ( char * );
void talker ( void );

void printf ( char *, ... );
void sprintf ( char *, char *, ... );

/* THE END */
