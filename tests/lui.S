/* lui test program
 * Jordan Horwich
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	lui s0, 0
	lui s1, UINT16_MAX
	lui s2, 1

	addiu t0, zero, UINT16_MAX
	ori s3, zero, UINT16_MAX

	lui s3, UINT16_MAX
	ori s4, s3, UINT16_MAX

quit:
	li		v0,	10
	syscall
