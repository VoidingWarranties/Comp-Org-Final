/* bne test program
 * Jordan Horwich
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	addi s0, zero, 0
	addi s1, zero, 0
	addi s2, zero, 0

	addi t0, zero, 0
	addi t1, zero, 0
	bne t0, t1, EQUAL_1
	addi s0, s0, -1
EQUAL_1:
	addi s0, s0, 1

	addi t0, zero, 0
	addi t1, zero, 1
	bne t0, t1, EQUAL_2
	addi s1, s1, -1
EQUAL_2:
	addi s1, s1, 1

	addi t0, zero, -1
	addi t1, zero, UINT16_MAX
	bne t0, t1, EQUAL_3
	addi s2, s2, -1
EQUAL_3:
	addi s2, s2, 1

quit:
	li	v0, 10
	syscall
