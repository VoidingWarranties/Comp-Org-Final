/* sltu test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

#define INT16_MAX 32767
#define INT16_MIN -32768

.globl __start
__start:
	addi	t0, zero, 0
	sltu	s0, zero, t0

	addi	t0, zero, -1
	sltu	s1, zero, t0

	addi	t0, zero, 1
	sltu	s2, zero, t0

	addi	t0, zero, -1
	addi	t1, zero, 0
	sltu	s3, t0, t1

	addi	t1, zero, -1
	sltu	s4, t0, t1

	addi	t1, zero, -2
	sltu	s5, t0, t1

	addi	t0, zero, 0
	addi	t1, zero, INT16_MAX

	sltu	s6, t0, t1
	sltu	s7, t1, t0

quit:
	li	v0, 10
	syscall
