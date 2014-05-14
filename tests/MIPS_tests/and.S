/* and test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	addi	t0, zero, 1
	addi	t1, zero, UINT16_MAX

	and	s0, zero, zero
	and	s1, zero, t0
	and	s2, zero, t1
	and	s3, t0, t0
	and	s4, t1, t1

	addi	t2, zero, 5

	and	s5, t0, t2
	and	s6, t1, t2

quit:
	li	v0, 10
	syscall
