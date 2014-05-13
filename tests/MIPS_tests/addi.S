/* addi test program
 * Jordan Horwich
 */

#include "registers.h"

.globl __start
__start:
	addi	s0, zero, 0
	addi	s1, zero, 1
	addi	s2, zero, -1
	addi	s3, s2, 1

quit:
	li	v0, 10
	syscall
