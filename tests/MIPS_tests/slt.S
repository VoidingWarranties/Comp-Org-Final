/* slt test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

#define INT16_MAX 32767
#define INT16_MIN -32768

.globl __start
__start:
	addi	t0, zero, 0
	slt	s0, zero, t0

	addi	t0, zero, -1
	slt	s1, zero, t0

	addi	t0, zero, 1
	slt	s2, zero, t0

	addi	t0, zero, -1
	addi	t1, zero, 0
	slt	s3, t0, t1

	addi	t1, zero, -1
	slt	s4, t0, t1

	addi	t1, zero, -2
	slt	s5, t0, t1

	addi	t0, zero, INT16_MIN
	addi	t1, zero, INT16_MAX

	slt	s6, t0, t1
	slt	s7, t1, t0

quit:
	li	v0, 10
	syscall
