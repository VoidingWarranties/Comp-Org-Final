/* addu test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

.globl __start
__start:
	addu	s0, zero, zero

	addi	t0, zero, 1
	addu	s1, zero, t0

	addi	t0, zero, -1
	addu	s2, zero, t0

	addu	s3, s2, s1

quit:
	li	v0, 10
	syscall
