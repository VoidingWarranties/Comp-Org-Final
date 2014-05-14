/* subu test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

.globl __start
__start:
	subu	s0, zero, zero

	addi	t0, zero, 1
	subu	s1, zero, t0

	addi	t0, zero, -1
	subu	s2, zero, t0

	subu	s3, s2, s1
	subu	s4, s1, s2

quit:
	li	v0, 10
	syscall
