/* led.c
 * access the on board LED on a Pi Pico board
 * (rp2040 chip)
 *
 * Tom Trebisky  9-15-2023
 */

typedef unsigned int u32;
typedef volatile unsigned int vu32;

/* Offsets for the four "IO flavors" */
#define FL_RW           0x0000
#define FL_XOR          0x1000
#define FL_SET          0x2000
#define FL_CLR          0x3000

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

#define SIO_BASE                    0xD0000000

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

/* This is just one of 32 bits */
#define R_IO_BANK0      (1<<5)

#define GPIO_25      (1<<25)

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

static inline void
set_io_func ( int gpio, int func )
{
        struct io_bank0 *iop;

        iop = (struct io_bank0 *) IO_BANK0_BASE_RW;
        iop->io[gpio].ctrl = func;
}

static struct sio *sp;

void
led_init ( void )
{
	do_reset ( R_IO_BANK0 );

	set_io_func ( 25, 5 );

        sp = (struct sio *) SIO_BASE;

	sp->gpio_oe_set = GPIO_25;
}

void
led_on ( void )
{
	sp->gpio_out_set = GPIO_25;
}

void
led_off ( void )
{
	sp->gpio_out_clr = GPIO_25;
}

void
blinker ( void )
{
	led_init ();

	for ( ;; ) {
	    led_on ();
	}
}

#ifdef  notdef
void
my_main ( void )
{
	for ( ;; ) {
	    // on
	    sp->gpio_out_set = GPIO_25;
	    blink_delay ();
	    // off
	    sp->gpio_out_clr = GPIO_25;
	    blink_delay ();
	}
}
#endif

/* THE END */
