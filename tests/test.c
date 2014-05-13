#include <stdio.h>
#include <assert.h>

#include "../sim/sim.h"

void test_addi(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### addi tests start ##########################################\n");
	ReadELF("addi.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);	// addi s0, $zero, 0
	assert(ctx.regs[s1] == 1);	// addi s1, $zero, 1
	assert(ctx.regs[s2] == -1);	// addi s2, $zero, -1
	assert(ctx.regs[s3] == 0);	// addi s3, s2, 1
	printf("#################### addi tests finished  ######################################\n");
}

void test_addiu(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### addiu tests start ##########################################\n");
	ReadELF("addiu.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);	// addiu s0, $zero, 0
	assert(ctx.regs[s1] == 1);	// addiu s1, $zero, 1
	assert(ctx.regs[s2] == -1);	// addiu s2, $zero, -1
	assert(ctx.regs[s3] == 0);	// addiu s3, s2, 1
	printf("#################### addiu tests finished  ######################################\n");
}

void test_andi(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### andi tests start ##########################################\n");
	ReadELF("andi.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);			// andi s0, zero, UINT16_MAX
	assert(ctx.regs[s1] == 0);			// andi s1, zero, 1
	                          			// addiu t0, zero, UINT16_MAX
	assert(ctx.regs[s3] == UINT16_MAX);	// andi s3, t0, UINT16_MAX
	assert(ctx.regs[s4] == 1);			// andi s4, t0, 1
	assert(ctx.regs[s5] == 5);			// andi s5, t0, 5
	printf("#################### andi tests finished  ######################################\n");
}

int main()
{
	union mips_instruction inst;
	struct virtual_mem_region* memory = NULL;
	struct context ctx;

	test_addi(inst, memory, ctx);
	test_addiu(inst, memory, ctx);
	test_andi(inst, memory, ctx);

	printf("All tests completed successfully!\n");
	return 0;
}
