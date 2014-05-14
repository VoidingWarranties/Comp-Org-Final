/* or test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	addi	t0, zero, 1
	addi	t1, zero, UINT16_MAX

	or	s0, zero, zero
	or	s1, zero, t0
	or	s2, zero, t1
	or	s3, t0, t0
	or	s4, t1, t1

	addi	t1, zero, 8
	or	s5, t1, t0

	addi	t0, zero, 16
	or	s6, t1, t0

	addi	t0, zero, 8
	or	s7, t1, t0

quit:
	li	v0, 10
	syscall
