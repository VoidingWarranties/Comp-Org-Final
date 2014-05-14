/* xor test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	addi	t0, zero, 1
	addi	t1, zero, UINT16_MAX

	xor	s0, zero, zero
	xor	s1, zero, t0
	xor	s2, zero, t1
	xor	s3, t0, t0
	xor	s4, t1, t1

	addi	t1, zero, 8
	xor	s5, t1, t0

	addi	t0, zero, 16
	xor	s6, t1, t0

	addi	t0, zero, 8
	xor	s7, t1, t0

	addi	t0, zero, 25
	xor	s8, t1, t0

quit:
	li	v0, 10
	syscall
