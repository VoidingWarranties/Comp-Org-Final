/* bltz test program
 * Jordan Horwich
 */

#include "registers.h"

.globl __start
__start:
	addi s0, zero, 0
	addi s1, zero, 0
	addi s2, zero, 0
	addi s3, zero, 0
	addi s4, zero, 0

	addi t0, zero, 0
	bltz t0, test_1
	addi s0, s0, -1
test_1:
	addi s0, s0, 1

	addi t0, zero, 1
	bltz t0, test_2
	addi s1, s1, -1
test_2:
	addi s1, s1, 1

	addi t0, zero, -1
	bltz t0, test_3
	addi s2, s2, -1
test_3:
	addi s2, s2, 1

	lui t0, 0b0111111111111111
	ori t0, t0, 0b1111111111111111
	bltz t0, test_4
	addi s3, s3, -1
test_4:
	addi s3, s3, 1

	lui t0, 0b1000000000000000
	bltz t0, test_5
	addi s4, s4, -1
test_5:
	addi s4, s4, 1

quit:
	li	v0, 10
	syscall
