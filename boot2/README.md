RP2040 - boot2

September 5, 2023

I have two reasons for fooling around with this.
One is that I want to understand how the rp2040 boots in
the general case where a fairly large bit of code is in flash.
The second is that I always find it very interesting and instructive
to understand how a system boots.  I'll say it this way:
I don't really understand a system until I understand in detail
how it boots.

I expect this project to operate in 3 stages:

First I will just copy the "bin" file for the second stage boot from
one of the sdk-example builds I have.  This is a 240 byte object.
Then I will modify my mkuf2 program to inject this as the starting
block of a uf2 file, followed by whatever executable I want to run.

Second, I will copy the smallest set of "official" raspberry pi
source files to build exactly the same "bin" file myself, and
work up a makefile to do so.  The aim here is to get a small
an comprehensible set of source files that are all gathered
together in one place.  I will feel free to perform as much
butchery as I see fit in doing this.

Third, I will work up my own source code, mostly to demonstrate
that I have full understanding of what is going on, but also
to condense the code even further and to code it in my own style.
This makes me happy, whether or not it pleases anyone else.
