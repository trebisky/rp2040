/* protos.h
 */

void led_init ( void );
void led_on ( void );
void led_off ( void );
void blinker ( void );

void do_reset ( int );
void pll_sys_reset ( void );

void io_delay ( void );
void tick_delay ( void );

void uart_init ( void );
void uart_puts ( char * );
void talker ( void );

void printf ( char *, ... );
void sprintf ( char *, char *, ... );

int digit1 ( int * );
int digit2 ( int * );

int get_cpuid ( void );

void core_start ( void );
void systick_init ( void );

void pll_init ( void );
void pll_show ( void );

void clock_init ( void );
void clock_show_all ( void );
void clock_sys_pll ( void );

/* THE END */
