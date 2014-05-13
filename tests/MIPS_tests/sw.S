/* sw test program
 * Jordan Horwich
 */

#include "registers.h"

.globl __start
__start:
	addi t0, zero, 1
	sw t0, -4(sp)

	addi t0, zero, 2
	sw t0, -8(sp)

	addi t0, zero, -1
	sw t0, -12(sp)

quit:
	li	v0, 10
	syscall
