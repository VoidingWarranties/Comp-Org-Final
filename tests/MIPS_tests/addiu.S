/* addiu test program
 * Jordan Horwich
 */

#include "registers.h"

.globl __start
__start:
	addiu	s0, zero, 0
	addiu	s1, zero, 1
	addiu	s2, zero, -1
	addiu	s3, s2, 1

quit:
	li	v0, 10
	syscall
