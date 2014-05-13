/* slti test program
 * Jordan Horwich
 */

#include "registers.h"

#define INT16_MAX 32767
#define INT16_MIN -32768

.globl __start
__start:
	addi	t0, zero, 0

	slti	s0, t0, 0
	slti	s1, t0, -1
	slti	s2, t0, 1

	addi	t0, zero, -1

	slti	s3, t0, 0
	slti	s4, t0, -1
	slti	s5, t0, -2

	addi	t0, zero, INT16_MIN

	slti	s6, t0, INT16_MAX

	addi	t0, zero, INT16_MAX

	slti	s7, t0, INT16_MIN

quit:
	li	v0, 10
	syscall
