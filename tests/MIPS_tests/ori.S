/* ori test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	ori	s0, zero, UINT16_MAX
	ori	s1, zero, 1

	addiu	t0, zero, UINT16_MAX

	ori	s2, t0, UINT16_MAX
	ori	s3, t0, 1

	addiu	t0, zero, 8

	ori	s4, t0, 1
	ori	s5, t0, 16
	ori	s6, t0, 8

quit:
	li	v0, 10
	syscall
