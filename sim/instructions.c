#include "instructions.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I-type

int addiu(uint32_t rt, uint32_t rs, uint32_t imm, struct context* ctx)
{
	if (rt == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rt] = ctx->regs[rs] + imm;
	return 1;
}
