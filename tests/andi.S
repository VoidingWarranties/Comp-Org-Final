/* andi test program
 * Jordan Horwich
 */

#include "registers.h"

#define UINT16_MAX 65535

.globl __start
__start:
	andi	s0, zero, UINT16_MAX
	andi	s1, zero, 1
	addiu	t0, zero, UINT16_MAX
	andi	s3, t0, UINT16_MAX
	andi	s4, t0, 1
	andi	s5, t0, 5
	andi	s6, zero, 0

quit:
	li		v0,	10
	syscall
