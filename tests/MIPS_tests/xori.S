/* xori test program
 * Jordan Horwich
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	xori s0, zero, UINT16_MAX
	xori s1, zero, 1

	addiu t0, zero, UINT16_MAX

	xori s2, t0, UINT16_MAX
	xori s3, t0, 1

	addiu t0, zero, 8

	xori s4, t0, 1
	xori s5, t0, 16
	xori s6, t0, 8
	xori s7, t0, 25

quit:
	li		v0,	10
	syscall
