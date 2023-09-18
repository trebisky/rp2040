/* printf.c
 * (c) Tom Trebisky  7-2-2017, 9-15-2023
 *
 * Taken from my EBAZ (Zynq)  project
 * Taken from my STM32F11 project
 *
 * This implements a simple and limited printf.
 *
 *  %s prints a string
 *  %c prints a single byte
 *  %x prints a byte as hex (2 digits)
 *  %h or %X prints a 32 bit integer as hex (8 digits)
 *  %d prints a decimal number (if you are lucky).
 */

#include <stdarg.h>

#include "protos.h"

#define PRINTF_BUF_SIZE 128
static void asnprintf (char *abuf, unsigned int size, const char *fmt, va_list args);

void
printf ( char *fmt, ... )
{
	char buf[PRINTF_BUF_SIZE];
        va_list args;

        va_start ( args, fmt );
        asnprintf ( buf, PRINTF_BUF_SIZE, fmt, args );
        va_end ( args );

        uart_puts ( buf );
}

/* The limit is absurd, so take care */
void
sprintf ( char *buf, char *fmt, ... )
{
        va_list args;

        va_start ( args, fmt );
        asnprintf ( buf, 256, fmt, args );
        va_end ( args );
}

/* ========================================================================= */

/* Here I develop a simple printf.
 * It only has 3 triggers:
 *  %s to inject a string
 *  %d to inject a decimal number
 *  %h to inject a 32 bit hex value as xxxxyyyy
 */

#define PUTCHAR(x)      if ( buf <= end ) *buf++ = (x)

static const char hex_table[] = "0123456789ABCDEF";

// #define HEX(x)  ((x)<10 ? '0'+(x) : 'A'+(x)-10)
#define HEX(x)  hex_table[(x)]

#ifdef notdef
static char *
sprintnb ( char *buf, char *end, int n, int b)
{
        char prbuf[16];
        register char *cp;

        if (b == 10 && n < 0) {
            PUTCHAR('-');
            n = -n;
        }
        cp = prbuf;

        do {
            // *cp++ = "0123456789ABCDEF"[n%b];
            *cp++ = hex_table[n%b];
            n /= b;
        } while (n);

        do {
            PUTCHAR(*--cp);
        } while (cp > prbuf);

        return buf;
}
#endif

/* 9-16-2023
 *
 * Who would expect so much trouble from %d in printf?
 *  The basic problem is that the ARM does not have a divide
 *  instruction.  So libgcc.a provides two functions to provide
 *  the missing functionality:
 *    __aeabi_idiv and __aeabi_idivmod
 *  So the first issue is working out the link library stuff in the
 *  Makefile to link against libgcc.a
 * This leads to the next problem.  Apparently these functions use
 *  some non-existant instruction (probably a multiply, but I haven't
 *  tracked down the exact details).  This will yield an ARM illegal
 *  instruction exception, which will probably be caught by the
 *  vector table in the bootrom (since I have not set up vectors
 *  of my own).  My guess is that the bootrom spins on an illegal
 *  instruction.  What I do know is that the game skids to a halt
 *  on the first call to one of the aeabi_*div routines.
 *
 * This actually leads to something more interesting.
 *  The rp2040 actually has division hardware.
 *  It is unique and not available as an ARM instruction.
 *  It is a "peripheral" in the SIO section.
 *  It might be fun, or at least interesting to write
 *  some assembly code to use this.
 */

// #define DIVIDE_WORKS

#ifdef DIVIDE_WORKS
/* The following code is how you would write sprintn if you
 * had a working compiler and divide instruction.
 */
static char *
sprintn ( char *buf, char *end, int n )
{
        char prbuf[16];
        char *cp;

	// printf ( "sprintn %X\n", n );
        if ( n < 0 ) {
            PUTCHAR('-');
            n = -n;
        }
        cp = prbuf;
	// uart_puts ( "sprintn A\n" );

        do {
            // *cp++ = "0123456789"[n%10];
            *cp++ = hex_table[n%10];
	    // uart_puts ( "sprintn B\n" );
            n /= 10;
	    // uart_puts ( "sprintn C\n" );
        } while (n);

        do {
            PUTCHAR(*--cp);
        } while (cp > prbuf);

        return buf;
}
#endif

#define DIVIDE_HACK

#ifdef DIVIDE_HACK
/* Once we divide by 10, the mod is easy,
 * we multiply by 10 and subtract.
 */
int
digit ( int *val )
{
	*val = 0;
	return 9;
}

static char *
sprintn ( char *buf, char *end, int n )
{
        char prbuf[16];
        char *cp;
	int d;

	// printf ( "sprintn %X\n", n );
        if ( n < 0 ) {
            PUTCHAR('-');
            n = -n;
        }
        cp = prbuf;
	// uart_puts ( "sprintn A\n" );

        do {
            // *cp++ = "0123456789"[n%10];
#ifdef notdef
	    d = digit2 ( &n );

	    /* Rotating bits example */
	    // asm ( code : out, in, clobber );
	    // asm("mov %[result], %[value], ror #1" : [result] "=r" (y) : [value] "r" (x));
	    asm volatile (
		"movs	%[digit], #0x7\n\t" 
		"movs	%[value], #0x0\n\t" 
		: [digit] "=r" (d)
		, [value] "=r" (n) : : "r1"
	    );
#endif

// #define SIO_BASE    0xD0000000

	    asm volatile (
		"ldr	r1, =0xD0000000\n\t"
		"str	%[value], [r1,#0x60]\n\t"
		"movs	r2, #10\n\t"
		"str	r2, [r1,#0x64]\n\t"

		// Delay for 8 cycles
		"b 1f\n\t"
		"1: b 1f\n\t"
		"1: b 1f\n\t"
		"1: b 1f\n\t"
		"1:"

		"ldr	%[digit], [r1,#0x74]\n\t"  // remainder
		"ldr	%[value], [r1,#0x70]\n\t"  // quotient
		: [digit] "=r" (d)
		, [value] "+r" (n) : : "r1", "r2"
	    );

            *cp++ = hex_table[d];
	    // uart_puts ( "sprintn B\n" );
            //n /= 10;
	    // uart_puts ( "sprintn C\n" );
        } while (n);

	// uart_puts ( "sprintn D\n" );
        do {
            PUTCHAR(*--cp);
        } while (cp > prbuf);

        return buf;
}
#endif

static char *
shex2( char *buf, char *end, int val )
{
        PUTCHAR( HEX((val>>4)&0xf) );
        PUTCHAR( HEX(val&0xf) );
        return buf;
}

#ifdef notdef
static char *
shex3( char *buf, char *end, int val )
{
        PUTCHAR( HEX((val>>8)&0xf) );
        return shex2(buf,end,val);
}

static char *
shex4( char *buf, char *end, int val )
{
        buf = shex2(buf,end,val>>8);
        return shex2(buf,end,val);
}
#endif

static char *
shex8( char *buf, char *end, int val )
{
        buf = shex2(buf,end,val>>24);
        buf = shex2(buf,end,val>>16);
        buf = shex2(buf,end,val>>8);
        return shex2(buf,end,val);
}

static void
asnprintf (char *abuf, unsigned int size, const char *fmt, va_list args)
{
    char *buf, *end;
    int c;
    char *p;

    buf = abuf;
    end = buf + size - 1;
    if (end < buf - 1) {
        end = ((void *) -1);
        size = end - buf + 1;
    }

    while ( c = *fmt++ ) {
	if ( c != '%' ) {
            PUTCHAR(c);
            continue;
        }
	c = *fmt++;
	if ( c == 'd' ) {
	    buf = sprintn ( buf, end, va_arg(args,int) );
	    continue;
	}
	if ( c == 'x' ) {
	    buf = shex2 ( buf, end, va_arg(args,int) & 0xff );
	    continue;
	}
	if ( c == 'h' || c == 'X' ) {
	    buf = shex8 ( buf, end, va_arg(args,int) );
	    continue;
	}
	if ( c == 'c' ) {
            PUTCHAR( va_arg(args,int) );
	    continue;
	}
	if ( c == 's' ) {
	    p = va_arg(args,char *);
	    // printf ( "Got: %s\n", p );
	    while ( c = *p++ )
		PUTCHAR(c);
	    continue;
	}
    }
    if ( buf > end )
	buf = end;
    PUTCHAR('\0');
}

#ifdef notdef
void
serial_printf ( int fd, char *fmt, ... )
{
	char buf[PRINTF_BUF_SIZE];
        va_list args;

        va_start ( args, fmt );
        asnprintf ( buf, PRINTF_BUF_SIZE, fmt, args );
        va_end ( args );

        uart_puts ( fd, buf );
}
#endif

/* Handy now and then */
void
show_reg ( char *msg, int *addr )
{
	printf ( "%s %h %h\n", msg, (int) addr, *addr );

	/*
	console_puts ( msg );
	console_putc ( ' ' );
	print32 ( (int) addr );
	console_putc ( ' ' );
	print32 ( *addr );
	console_putc ( '\n' );
	*/
}

/* THE END */
