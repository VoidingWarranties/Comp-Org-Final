#include "instructions.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R-type

int add(uint32_t rs, uint32_t rt, uint32_t rd, struct context* ctx)
{
	if (rd == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rd] = ctx->regs[rs] + ctx->regs[rt];
	return 1;
}

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

int andi(uint32_t rt, uint32_t rs, uint32_t imm, struct context* ctx)
{
	if (rt == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rt] = ctx->regs[rs] & imm;
	return 1;
}

int ori(uint32_t rt, uint32_t rs, uint32_t imm, struct context* ctx)
{
	if (rt == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rt] = ctx->regs[rs] | imm;
	return 1;
}

int xori(uint32_t rt, uint32_t rs, uint32_t imm, struct context* ctx)
{
	if (rt == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rt] = ctx->regs[rs] ^ imm;
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory load / store

int lui(uint32_t rt, uint32_t imm, struct context* ctx)
{
	if (rt == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	ctx->regs[rt] = imm << 16;
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
