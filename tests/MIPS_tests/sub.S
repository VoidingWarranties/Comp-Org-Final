/* sub test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

.globl __start
__start:
	sub	s0, zero, zero

	addi	t0, zero, 1
	sub	s1, zero, t0

	addi	t0, zero, -1
	sub	s2, zero, t0

	sub	s3, s2, s1
	sub	s4, s1, s2

quit:
	li	v0, 10
	syscall
