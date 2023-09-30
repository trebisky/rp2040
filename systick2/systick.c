/* systick.c
 * Experiment with the systick timer
 * (rp2040 chip)
 *
 * Tom Trebisky  9-25-2023
 */

#include "protos.h"

typedef unsigned int u32;
typedef volatile unsigned int vu32;

/* Offsets for the four "IO flavors" */
#define FL_RW           0x0000
#define FL_XOR          0x1000
#define FL_SET          0x2000
#define FL_CLR          0x3000

/* Systick is a 24 bit down counter.
 * It can either count a 1 us pulse (1 Mhz?)
 * or be clocked by the system clock
 */

struct cpu {
	u32	_pad[4];

	vu32	syst_csr;	/* 0xE010 */
	vu32	syst_rvr;	// reload register
	vu32	syst_cvr;	// current value
	vu32	syst_calib;
};

/* For reasons unknown to anyone, the datasheet and SDK call
 * this PPB_BASE.  Not only that, but there is a huge gap at
 * the start of the address space, so I set up a base address
 * at a more useful location.
 */
// #define CPU_BASE                    ((struct cpu *) 0xE0000000)
#define CPU_BASE                    ((struct cpu *) 0xE000E000)

/* This address space is unique to each core (each core has
 * its own private copy at the same address)
 * So each core has its own systick, nvic, vtor, and so forth.
 */

#define CSR_FLAG	0x10000		// clears on read
#define CSR_SOURCE	0x4		// set is processor, clear is ref
#define CSR_INT		0x2		// enable interrupt
#define CSR_ENA		0x1		// enable the counter

/* Are these really read only ?? */
#define CAL_NOREF	0x80000000	// RO - force source to 1
#define CAL_SKEW	0x40000000	// RO - 10 ms cal is inexact
#define CAL_TENMS	0x00ffffff	// RO - reload value

/* With "tick_delay()" "pulled out of the air" it takes 23 seconds
 * to run these 1000 iterations.
 * So each itermation is 23/1000 = .023 seconds (23 milliseconds)
 * We see the systick value change by 265,522 counts.
 * This would suggest a 11.544 Mhz clock (12 Mhz)
 *  (using the processor clock)
 *
 * What happens if we use the "reference" clock?
 *  Now we see a change of 22,138 between samples.
 * This suggests 0.962 Mhz (1 Mhz) from whatever
 *  the "reference clock" might be.
 *
 * The board does have a 12 Mhz crystal oscillator, so the first
 *  measurement suggests that the CPU is simply running without
 *  any PLL setup at the basic crystal frequency.
 *
 * Note that a 24 bit counter allows us up to a 16M count.
 *
 * After I got the clock speed bumped to 125 Mhz, my 1000 iterations
 * are running in 3 seconds, which is certainly encouraging.
 * I switch to using 10000 iterations to get better accuracy from
 * my stopwatch (and thumb).  With 10000 iterations, my stopwatch
 * measures 25 seconds (so .0025 seconds per sample).
 * I count 304062 systick counts in that time.
 * This gives 121.6 Mhz, which sounds enough like 125 Mhz for me.
 *
 * Why do I measure the reference clock to systick as 1 Mhz?
 * As near as I can tell the clock hardware sets this up to use
 * the raw crystal oscillator, which is 12 Mhz.
 */

void
systick_calib ( void )
{
	struct cpu *cp = CPU_BASE;
	int limit = 10000;
	// int limit = 1000;
	// int limit = 100;
	// int limit = 10;
	u32 val;

	// for ( ;; ) {
	while ( limit-- ) {
	    tick_delay ();
	    val = cp->syst_cvr;
	    printf ( "systick = %X %d\n", val, val );
	}
}

// #define SEV()           asm volatile ( "sev\r\n" )
#define INT_enable()           asm volatile ( "cpsie i" : : : "memory")
#define INT_disable()          asm volatile ( "cpsid i" : : : "memory")

static int tick_count;

/* Can be called by either core */
void
systick_init ( void )
{
	struct cpu *cp = CPU_BASE;

	cp->syst_rvr = 0xff0000;

	// Use processor clock (sys_clk)
	cp->syst_csr = CSR_SOURCE | CSR_ENA;

	// Use "reference" clock
	// cp->syst_csr = CSR_ENA;

	// systick_calib ();

	clock_init ();

	/* Somewhat to my surprise, this does disable/enable the
	 * systick interrupt!  A good thing ultimately, but I had
	 * thought that perhaps PRIMASK would only affect actual
	 * interrupt sources coming through the NVIC.
	 * Note also that interrupts are enabled already,
	 * the call below to INT_enable() is not necessary.
	 */
	// INT_disable();
	INT_enable();

	tick_count = 0;
	cp->syst_csr = CSR_SOURCE | CSR_INT | CSR_ENA;
}

/* Called when we get a systick interrupt from Core 0.
 */
void
c0_systick_isr ( void )
{
	struct cpu *cp = CPU_BASE;

	printf ( "TICK (core 0) %d\n", tick_count++ );

	if ( tick_count > 10 ) {
	    cp->syst_csr = CSR_SOURCE | CSR_ENA;
	    printf ( "Done with systick interrupts for core 0\n" );
	}
}

void
c1_systick_isr ( void )
{
	struct cpu *cp = CPU_BASE;

	printf ( "TICK (core 1) %d\n", tick_count++ );

	if ( tick_count > 10 ) {
	    cp->syst_csr = CSR_SOURCE | CSR_ENA;
	    printf ( "Done with systick interrupts for core 1\n" );
	}
}

// ===============================================================

/* This doesn't really belong in this file, but here it is for now.
*/

struct pll {
	vu32	cs;
	vu32	pwr;
	vu32	div;
	vu32	post;
};

#define PLL_SYS_BASE	((struct pll *) 0x40028000)
#define PLL_USB_BASE	((struct pll *) 0x4002c000)

#define CSR_LOCK	0x80000000

/* There are 4 power down bits in the PWR register.
 */
#define PWR_VCO		0x20
#define PWR_POST	0x08
#define PWR_DSM		0x04
#define PWR_PLL		0x01

/* Here is what we see coming out of the bootrom --
 *  PLL cs   = 00000001
 *  PLL pwr  = 0000002D
 *  PLL div  = 00000000
 *  PLL post = 00077000
 *
 * The PLL is not locked and the refdiv is 1
 * The VCO and dividers are powered down
 * The divider is zero
 * The post dividers are 7*7
 * 
 * These are all reset values, so the PLL has
 * not been initialized at all by the bootrom.
 */

void
pll_show ( void )
{
	struct pll *pp = PLL_SYS_BASE;

	printf ( "PLL cpuid= %d\n", get_cpuid() );
	printf ( "PLL cs   = %X\n", pp->cs );
	printf ( "PLL pwr  = %X\n", pp->pwr );
	printf ( "PLL div  = %X\n", pp->div );
	printf ( "PLL post = %X\n", pp->post );
}

static int
pll_wait ( void )
{
	struct pll *pp = PLL_SYS_BASE;

	int tmo = 9900;

	while ( tmo-- )
	    if ( pp->cs & CSR_LOCK )
		return 1;
	return 0;
}

static void
pll_setup_125 ( void )
{
	struct pll *pp = PLL_SYS_BASE;

	pll_sys_reset ();

	/* Don't divide the 12 Mhz reference clock
	 */
	pp->cs = 1;

	/* We want the vco to run at 1500 Mhz
	 * 12 * 125 = 1500
	 */
	pp->div = 125;

	/* Clear PLL and VCO bits to power them up
	 */
	pp->pwr = PWR_POST | PWR_DSM;

	if ( ! pll_wait() )
	    printf ( "PLL timeout waiting for lock\n" );

	pp->post = 6<<16 | 2<<12;

	/* Power on the post divider
	 */
	pp->pwr = PWR_DSM;
}

void
pll_init ( void )
{
	pll_show ();
	pll_setup_125 ();
	pll_show ();
	clock_sys_pll ();
	clock_show_all ();
}

// ===============================================================

struct clock {
	vu32	ctrl;
	vu32	div;
	vu32	sel;
};

struct clocks {
	struct clock clk[10];
};

/* Note the "8" in the following */
#define CLOCK_BASE	( (struct clocks *) 0x40008000 )

/* settings for the aux source field
 */
#define SRC_PLL_SYS	0
#define SRC_PLL_USB	1
#define SRC_ROSC	2
#define SRC_XOSC	3
#define SRC_GPIN0	4
#define SRC_GPIN1	5

/* index for which clock we are messing with
 */
#define CLK_GPOUT0	0
#define CLK_GPOUT1	1
#define CLK_GPOUT2	2
#define CLK_GPOUT3	3
#define CLK_REF		4
#define CLK_SYS		5
#define CLK_PERI	6
#define CLK_USB		7
#define CLK_ADC		8
#define CLK_RTC		9

/* Here is what we see coming out of the bootrom:
 *  - Ref clock --
 *  CLK ctrl = 00000002
 *  CLK div  = 00000100
 *  CLK sel  = 00000004
 *  - Sys clock --
 *  CLK ctrl = 00000000
 *  CLK div  = 00000100
 *  CLK sel  = 00000001
 *
 * For the Ref clock we see the XOSC as the glitchless source
 * The divider is set to 1
 * The sel register shows 4, which is confusing
 * For the Sys clock we see the ref clock as the glitchless source
 * The divisoor is again set to 1
 * The sel register shows 1
 */

static void
clock_show ( int index )
{
	struct clocks *ccp = CLOCK_BASE;
	struct clock *cp;

	cp = &ccp->clk[index];

	printf ( "CLK ctrl = %X\n", cp->ctrl );
	printf ( "CLK div  = %X\n", cp->div );
	printf ( "CLK sel  = %X\n", cp->sel );
}

void
clock_show_all ( void )
{
	printf ( "- Ref clock --\n" );
	clock_show ( CLK_REF );
	printf ( "- Sys clock --\n" );
	clock_show ( CLK_SYS );
}

void
clock_init ( void )
{
	clock_show_all ();
}

/* Switch the CPU (sys) clock to use the PLL
 * The aux source field is already set to 0
 *  (to select the PLL), so all we need to do
 *  is to set the glitchless bit to 1
 */
void
clock_sys_pll ( void )
{
	struct clocks *ccp = CLOCK_BASE;
	struct clock *cp;

	cp = &ccp->clk[CLK_SYS];

	cp->ctrl = 1;
}

/* THE END */
