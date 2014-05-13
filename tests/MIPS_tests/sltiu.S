/* sltiu test program
 * Jordan Horwich
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	addi t0, zero, 0

	sltiu s0, t0, 0
	sltiu s1, t0, -1
	sltiu s2, t0, 1

	addi t0, zero, -1

	sltiu s3, t0, 0
	sltiu s4, t0, -1
	sltiu s5, t0, -2

	addi t0, zero, 0

	sltiu s6, t0, UINT16_MAX

	addi t0, zero, UINT16_MAX

	sltiu s7, t0, 0

quit:
	li		v0,	10
	syscall
