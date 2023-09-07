/* mkuf2.c
 * convert a binary image into a uf2 file
 * that will be booted onto the rp2040 (Pi Pico)
 * Tom Trebisky  8-16-2023
 *
 * This is version 1 - it does not handle the stage 2 boot
 *  So it can only put a 252 byte thing into flash
 *    (masquerading as the stage 2 boot)
 *  Or it can load a bigger thing into SRAM, where
 *    it will run, but vanish on reset or power cycle.
 *
 * Modeled after the utility written by David Welch
 * 
 * No overall CRC (doesn't seem to be needed)
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SSIZE	256	// sector (block) size

void
error ( char *msg )
{
	fprintf ( stderr, "%s\n", msg );
	exit ( 1 );
}

void
usage ( void )
{
	error ( "Usage: mkuf2 [-s] binfile uf2file" );
}

typedef unsigned int u32;
typedef unsigned char u8;

/* More or less from the Microsoft document */
struct uf2_block {
    // 32 byte header
    u32 magicStart0;
    u32 magicStart1;
    u32 flags;
    u32 targetAddr;
    u32 payloadSize;
    u32 blockNo;
    u32 numBlocks;
    u32 familyID; 	// or fileSize
    char   data[476];
    u32 magicEnd;
};

/* Flags -- */
#define F_SKIP		0x0001	/* This block has comments or something */
#define F_CONTAINER	0x1000	/* File container*/
#define F_FAMILY	0x2000	/* CPU family in familyID field */
#define F_MD5		0x4000	/* */
#define F_EXTENSION	0x8000	/* extension tags present*/

#define MAX_IMAGE	256*1024
#define MAX_READ	246*1024

u8 image[MAX_IMAGE];

enum mem { FLASH, SRAM };

#define FLASH_BASE	0x10000000
#define SRAM_BASE	0x20000000

u32 crc32 ( u8 *, int );

int main ( int argc, char **argv )
{
	struct uf2_block block;
	char *p;
	int fd;
	int nread;
	int nblocks;
	int i;
	int off;
	enum mem target_type;
	u32 target;
	u32 *crcp;

	if ( argc < 3 )
	    usage ();

	target_type = FLASH;

	argc--;
	argv++;
	if ( **argv == '-' ) {
	    p = *argv;
	    if ( p[1] == 's' )
		target_type = SRAM;
	    argc--;
	    argv++;
	}
	if ( argc != 2 )
	    usage ();

	fd = open ( argv[0], O_RDONLY );
	if ( fd < 0 )
	    error ( "Cannot open input file" );
	nread = read ( fd, image, MAX_IMAGE );
	if ( nread <= 0 )
	    error ( "Cannot read input file" );
	if ( nread > MAX_READ )
	    error ( "input file too big" );
	close ( fd );

	nblocks = (nread + SSIZE-1) / SSIZE;

	printf ( "Read: %d bytes (%d blocks)\n", nread, nblocks );

	/* The idea here is that the bootloader works by looking
	 * for a 256 byte sector with a valid checksum.
	 * If it finds it, it will run it.
	 * This works for small demos (for us).
	 * For bigger things, this is supposed to be a second stage loader
	 * that will pull in the bigger thing.
	 * We can load demos of any size into SRAM without this
	 * limitation, and without needing a checksum
	 */
	if ( target_type == FLASH && nread > (SSIZE - 4) )
	    error ( "Too big to fit in flash all by itself" );

	if ( target_type == FLASH ) {
	    crcp = (u32 *) &image[SSIZE-4];
	    *crcp = crc32 ( image, SSIZE - 4 );
	}

	fd = open ( argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0644 );

	off = 0;
	target = (target_type == FLASH) ? FLASH_BASE : SRAM_BASE;

	if ( target_type == SRAM )
	    printf ( "Target = %08x (SRAM)\n", target );
	else
	    printf ( "Target = %08x (Flash)\n", target );

	for ( i=0; i<nblocks; i++ ) {
	    memset ( (char *) &block, 0x00, 512 );

	    block.magicStart0 = 0x0A324655;
	    block.magicStart1 = 0x9E5D5157;
	    block.flags = F_FAMILY;
	    block.targetAddr = target;
	    block.payloadSize = SSIZE;
	    block.blockNo = i;
	    block.numBlocks = nblocks;
	    block.familyID = 0xE48BFF56;

	    memcpy ( block.data , &image[off], SSIZE);

	    block.magicEnd = 0x0AB16F30;

	    write ( fd, (char *) &block, 512 );

	    off += SSIZE;
	    target += SSIZE;
	}

	close ( fd );

	return 0;
}

/* THE END */
