/* uart.c
 * Send endless characters from a Pi Pico
 * (rp2040 chip)
 *
 * Tom Trebisky  8-17-2023
 *
 * The final binary image is 232 bytes.
 *
 * I owe a debt to David Welch, I learned how to initialize the
 * clocks and uart from his code examples.
 *
 * This little demo runs in flash, as it fits into the 252 byte limit
 * so it can be packaged up as a second stage boot loader that the
 * rp2040 will load and run from flash.
 *
 * While debugging I used the LED code (now commented out) and with that
 * it was too big to fit, so I ran it from SRAM
 */

#include "protos.h"

typedef unsigned int u32;
typedef volatile unsigned int vu32;

#define	BIT(n)	(1 << (n))

/* Offsets for the four "IO flavors" */
#define FL_RW           0x0000
#define FL_XOR          0x1000
#define FL_SET          0x2000
#define FL_CLR          0x3000

/* ----------------------------------- */

struct clocks {
    vu32         gpout0_ctrl;
    vu32         gpout0_div;
    vu32         gpout0_sel;
    vu32         gpout1_ctrl;
    vu32         gpout1_div;
    vu32         gpout1_sel;
    vu32         gpout2_ctrl;
    vu32         gpout2_div;
    vu32         gpout2_sel;
    vu32         gpout3_ctrl;
    vu32         gpout3_div;
    vu32         gpout3_sel;
    vu32         ref_ctrl;
    vu32         ref_div;
    vu32         ref_sel;
    vu32         sys_ctrl;
    vu32         sys_div;
    vu32         sys_sel;
    vu32         peri_ctrl;
    vu32         peri_div;
    vu32         peri_sel;
    vu32         usb_ctrl;
    vu32         usb_div;
    vu32         usb_sel;
    vu32         rtc_ctrl;
    vu32         rtc_div;
    vu32         rtc_sel;

    vu32         sys_resus_ctrl;
    vu32         sys_resus_status;
    vu32         fc0_ref_khz;
    vu32         fc0_min_khz;
    vu32         fc0_max_khz;
    vu32         fc0_delay;
    vu32         fc0_interval;
    vu32         fc0_src;
    vu32         fc0_status;
    vu32         fc0_result;
    vu32         wake_en0;
    vu32         wake_en1;
    vu32         sleep_en0;
    vu32         sleep_en1;
    vu32         enabled0;
    vu32         enabled1;

    vu32         intr;
    vu32         inte;
    vu32         intf;
    vu32         ints;
};

/* Stuff in peri_ctrl */
#define PERI_ENABLE	BIT(11)
#define PERI_SRC_XOSC	(4<<5)

#define CLOCKS_BASE                 0x40008000
#define CLOCKS_BASE_RW		(struct clocks *) (CLOCKS_BASE + FL_RW)

/* ----------------------------------- */
/* Crystal Oscillator */

struct xosc {
	vu32	ctrl;
	vu32	status;
	vu32	dormant;;
	vu32	startup;
	vu32	count;
};

#define XOSC_BASE		0x40024000
#define XOSC_BASE_RW		(struct xosc *) (XOSC_BASE + FL_RW)
#define XOSC_BASE_SET		(struct xosc *) (XOSC_BASE + FL_SET)

/* ----------------------------------- */

#define IO_BANK0_BASE               0x40014000
#define IO_BANK0_BASE_RW	(IO_BANK0_BASE + FL_RW)
#define IO_BANK0_BASE_XOR	(IO_BANK0_BASE + FL_XOR)
#define IO_BANK0_BASE_SET	(IO_BANK0_BASE + FL_SET)
#define IO_BANK0_BASE_CLR	(IO_BANK0_BASE + FL_CLR)

struct io_bank0 {
	struct {
	    vu32	status;
	    vu32 	ctrl;
	} io[30];
};

/* ----------------------------------- */
/* Not serial IO, but "single cycle IO" or some such */
struct sio {
    u32		_pad[4];
    vu32		gpio_out_rw;
    vu32		gpio_out_set;
    vu32		gpio_out_clr;
    vu32		gpio_out_xor;

    vu32		gpio_oe_rw;
    vu32		gpio_oe_set;
    vu32		gpio_oe_clr;
    vu32		gpio_oe_xor;
};

#define SIO_BASE                    (struct sio *) 0xD0000000

/* ----------------------------------- */

/* various bits in the reset control register */
#define R_IO_BANK0      (1<<5)
#define R_UART0		(1<<22)

#define GPIO_25      (1<<25)

// This blinks at about 1 Hz
#define BLINK_DELAY_COUNT	0x80000
#define TICK_DELAY_COUNT	0x8000

// #define TALK_DELAY_COUNT	0x20000
#define TALK_DELAY_COUNT	0x120000

#ifdef notdef
void
blink_delay ( void )
{
	// volatile int delay = BLINK_DELAY_COUNT;
	volatile int delay = BLINK_DELAY_COUNT;

	while ( delay-- )
	    ;
}
#endif

void
tick_delay ( void )
{
	volatile int delay = TICK_DELAY_COUNT;

	while ( delay-- )
	    ;
}

void
io_delay ( void )
{
	// volatile int delay = BLINK_DELAY_COUNT;
	volatile int delay = TALK_DELAY_COUNT;

	while ( delay-- )
	    ;
}

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

void
do_reset ( int who )
{
        struct resets *rp;

	rp = (struct resets *) RESET_BASE_CLR;
        rp->reset = who;

        rp = (struct resets *) RESET_BASE_RW;
        while ( (rp->done & who) == 0 )
            ;
}
#endif

static inline void
set_io_func ( int gpio, int func )
{
	struct io_bank0 *iop;

        iop = (struct io_bank0 *) IO_BANK0_BASE_RW;
	iop->io[gpio].ctrl = func;
}

/* bits in flags register */
#define	F_BUSY	BIT(3)
#define	F_RXFE	BIT(4)
#define	F_TXFF	BIT(5)

struct uart {
	vu32	data;
	vu32	rsr;
	u32	_pad0a;
	u32	_pad0b;
	u32	_pad0c;
	u32	_pad0d;
	vu32	flags;		/* 0x18 */
	u32	_pad1;
	vu32	ilpr;
	vu32	ibrd;
	vu32	fbrd;		/* 0x28 */
	vu32	lcr_h;
	vu32	lcr;
};

#define UART0_BASE                  0x40034000
#define UART0_BASE_RW	(struct uart *) (UART0_BASE + FL_RW)

void
uart_putc ( int ch )
{
	struct uart *up = UART0_BASE_RW;

	while ( up->flags & F_TXFF )
	    ;
	up->data = ch;
}

void
uart_puts ( char *s )
{
        while ( *s ) {
            if (*s == '\n')
                uart_putc('\r');
            uart_putc(*s++);
        }
}

void
uart_setup ( void )
{
	struct uart *up = UART0_BASE_RW;

	up->ibrd = 6;
	up->fbrd = 33;
	up->lcr_h = 0x70;
	up->lcr = BIT(0) | BIT(8) | BIT(9);
}

#ifdef notdef
static inline void
led_on ( void )
{
	struct sio *sp = SIO_BASE;;

	// led_init ();
	sp->gpio_out_set = GPIO_25;	// on
}

void
led_init ( void )
{
	struct sio *sp = SIO_BASE;;

	/* For onboard LED */
	/* Set function select to software IO */
	set_io_func ( 25, 5 );

	sp->gpio_oe_set = GPIO_25;
}

void
blinker ( void )
{
	struct sio *sp = SIO_BASE;

	for ( ;; ) {
	    sp->gpio_out_set = GPIO_25;	// on
	    blink_delay ();
	    sp->gpio_out_clr = GPIO_25; // off
	    blink_delay ();
	}
}

void
iam_here ( void )
{
	led_init ();
	blinker ();
}
#endif
void
clocks_init ( void )
{
	struct clocks *cp = CLOCKS_BASE_RW;
	struct xosc *xp = XOSC_BASE_RW;

	cp->sys_resus_ctrl = 0;

	/* Set up crystal oscillator */
	xp->ctrl = 0xaa0;
	xp->startup = 47;
	xp = XOSC_BASE_SET;
	xp->ctrl = 0x00fab000;
	xp = XOSC_BASE_RW;
	while ( ! (xp->status & 0x80000000) )
	    ;

	cp->ref_ctrl = 2;
	cp->sys_ctrl = 0;

	cp->peri_ctrl = PERI_ENABLE | PERI_SRC_XOSC;
}

void
uart_init ( void )
{
	/* reset IO Bank 0 - need here for LED */
	do_reset ( R_IO_BANK0 );

	clocks_init ();

	do_reset ( R_UART0 );
	uart_setup ();

	/* Select UART on pins 16 and 17 */
	set_io_func ( 16, 2 );
	set_io_func ( 17, 2 );

#ifdef notdef
	/* UART on pins 0 and 1 */
	set_io_func ( 0, 2 );
	set_io_func ( 1, 2 );
#endif

	// blinker ();
	// talker ();
}

/* THE END */

