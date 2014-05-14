/* sb test program
 * Jordan Horwich
 * Rocco Del Priore
 */

#include "registers.h"

.globl __start
__start:
	addi t0, zero, 0
	sb t0, 0(sp)

	addi t0, zero, 1
	sb t0, -1(sp)

	addi t0, zero, 2
	sb t0, -2(sp)

	addi t0, zero, -1
	sb t0, -3(sp)

quit:
	li	v0, 10
	syscall
