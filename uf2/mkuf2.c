/* makeuf2.c
 * convert a binary image into a uf2 file
 * that will be booted onto the rp2040 (Pi Pico)
 * Tom Trebisky  8-16-2023
 *
 * This is version 2 which handles a stage 2 boot
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
	error ( "Usage: mkuf2 [-s] [-b bootfile] binfile uf2file" );
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
    u32 payloadSize;	// 0x10
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

u8 boot_image[SSIZE];

int nblocks;
u8 image[MAX_IMAGE];

enum mem { SRAM, FLASH, FLASH2 };

#define FLASH_BASE		0x10000000
#define FLASH_BASE_CODE		0x10000100
#define SRAM_BASE		0x20000000

u32 crc32 ( u8 *, int );
void write_block ( int, int, u32, u8 * );
void write_ssb ( int, u8 * );

int main ( int argc, char **argv )
{
	struct uf2_block block;
	char *p;
	int fd;
	int nb_main;
	int block_num;
	int nread;
	int nboot;
	int i;
	int off;
	enum mem target_type;
	u32 target;

	if ( argc < 3 )
	    usage ();

	target_type = FLASH;

	argc--;
	argv++;

	while ( **argv == '-' ) {
	    p = *argv;
	    if ( p[1] == 's' ) {
		if ( target_type == FLASH2 )
		    error ( "Cannot have both -b and -s" );
		target_type = SRAM;
	    }
	    if ( p[1] == 'b' ) {
		if ( target_type == SRAM )
		    error ( "Cannot have both -b and -s" );
		target_type = FLASH2;
	    }
	    argc--;
	    argv++;
	}

	if ( target_type == FLASH2 ) {
	    if ( argc !=3 )
		usage ();
	} else {
	    if ( argc != 2 )
		usage ();
	}

	/* Read second stage boot image
	 *  if one was specified.
	 */
	if ( target_type == FLASH2 ) {
	    memset ( boot_image, 0x00, SSIZE );
	    fd = open ( argv[0], O_RDONLY );
	    if ( fd < 0 )
		error ( "Cannot open boot file" );
	    nboot = read ( fd, boot_image, SSIZE );
	    if ( nboot <= 0 )
		error ( "Cannot read boot file" );
	    if ( nread > (SSIZE-4) )
		error ( "boot file too big" );
	    close ( fd );

	    printf ( "SSB - read: %d bytes\n", nboot );
	    argc--;
	    argv++;
	}

	/* Read main image */
	fd = open ( argv[0], O_RDONLY );
	if ( fd < 0 )
	    error ( "Cannot open input file" );
	nread = read ( fd, image, MAX_IMAGE );
	if ( nread <= 0 )
	    error ( "Cannot read input file" );
	if ( nread > MAX_READ )
	    error ( "input file too big" );
	close ( fd );

	nb_main = (nread + SSIZE-1) / SSIZE;
	nblocks = nb_main;
	block_num = 0;
	if ( target_type == FLASH2 ) {
	    nblocks++;
	    block_num++;
	}

	printf ( "Image - read: %d bytes (%d blocks)\n", nread, nb_main );

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

	fd = open ( argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0644 );

	/* We will write only one block.
	 * Our executable masquerades as a second stage boot
	 */
	if ( target_type == FLASH ) {
	    write_ssb ( fd, image );
	    close ( fd );
	    return 0;
	}

	if ( target_type == FLASH2 ) {
	    write_ssb ( fd, boot_image );
	}

	if ( target_type == SRAM ) {
	    target = SRAM_BASE;
	    printf ( "Target = %08x (SRAM)\n", target );
	} else {
	    target = FLASH_BASE_CODE;
	    printf ( "Target = %08x (Flash)\n", target );
	}

	off = 0;
	for ( i=0; i<nb_main; i++ ) {
	    write_block ( fd, block_num, target, &image[off] );

	    block_num++;
	    off += SSIZE;
	    target += SSIZE;
	}

	close ( fd );

	return 0;
}

void
write_block ( int fd, int bnum, u32 target, u8 *data )
{
	struct uf2_block block;

	memset ( (char *) &block, 0x00, 512 );

	block.magicStart0 = 0x0A324655;
	block.magicStart1 = 0x9E5D5157;
	block.flags = F_FAMILY;
	block.targetAddr = target;
	block.payloadSize = SSIZE;
	block.blockNo = bnum;
	block.numBlocks = nblocks;
	block.familyID = 0xE48BFF56;

	memcpy ( block.data , data, SSIZE);

	block.magicEnd = 0x0AB16F30;

	write ( fd, (char *) &block, 512 );
}

/* Write second stage boot */
void
write_ssb ( int fd, u8 *data )
{
	u32 *crcp;

	crcp = (u32 *) &data[SSIZE-4];
	*crcp = crc32 ( data, SSIZE - 4 );

	write_block ( fd, 0, FLASH_BASE, data );
}

/* THE END */
