/* add test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

.globl __start
__start:
	add	s0, zero, zero

	addi	t0, zero, 1
	add	s1, zero, t0

	addi	t0, zero, -1
	add	s2, zero, t0

	add	s3, s2, s1

quit:
	li	v0, 10
	syscall
