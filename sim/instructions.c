#include "instructions.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I-type

int addi(uint32_t rt, uint32_t rs, uint32_t imm, struct context* ctx)
{
	if (rt == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rt] = ctx->regs[rs] + imm;
	return 1;
}

int lw(uint32_t rt, uint32_t rs, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx)
{
	if (rt == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rt] = FetchWordFromVirtualMemory(ctx->regs[rs] + imm, memory);
	return 1;
}

int sw(uint32_t rt, uint32_t rs, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx)
{
	StoreWordToVirtualMemory(ctx->regs[rs] + imm, ctx->regs[rt], memory);
	return 1;
}

int lb(uint32_t rt, uint32_t rs, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx)
{
	if (rt == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rt] = FetchByteFromVirtualMemory(ctx->regs[rs] + imm, memory);
	return 1;
}

int sb(uint32_t rt, uint32_t rs, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx)
{
	StoreByteToVirtualMemory(ctx->regs[rs] + imm, ctx->regs[rt], memory);
	return 1;
}
