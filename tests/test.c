#include <stdio.h>
#include <assert.h>

#include "../sim/sim.h"

void test_addi(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### addi tests start ##########################################\n");
	ReadELF("addi.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);  // addi s0, $zero, 0
	assert(ctx.regs[s1] == 1);  // addi s1, $zero, 1
	assert(ctx.regs[s2] == -1); // addi s2, $zero, -1
	assert(ctx.regs[s3] == 0);  // addi s3, s2, 1
	printf("#################### addi tests finished  ######################################\n");
}

void test_addiu(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### addiu tests start ##########################################\n");
	ReadELF("addiu.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);  // addiu s0, $zero, 0
	assert(ctx.regs[s1] == 1);  // addiu s1, $zero, 1
	assert(ctx.regs[s2] == -1); // addiu s2, $zero, -1
	assert(ctx.regs[s3] == 0);  // addiu s3, s2, 1
	printf("#################### addiu tests finished  ######################################\n");
}

void test_andi(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### andi tests start ##########################################\n");
	ReadELF("andi.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);          // andi s0, zero, UINT16_MAX
	assert(ctx.regs[s1] == 0);          // andi s1, zero, 1
	                                    // addiu t0, zero, UINT16_MAX
	assert(ctx.regs[s2] == UINT16_MAX); // andi s2, t0, UINT16_MAX
	assert(ctx.regs[s3] == 1);          // andi s3, t0, 1
	assert(ctx.regs[s4] == 5);          // andi s4, t0, 5
	assert(ctx.regs[s5] == 0);          // andi s5, zero, 0
	printf("#################### andi tests finished  ######################################\n");
}

void test_ori(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### ori tests start ##########################################\n");
	ReadELF("ori.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == UINT16_MAX); // ori s0, zero, UINT16_MAX
	assert(ctx.regs[s1] == 1);          // ori s1, zero, 1
	                                    // addiu t0, zero, UINT16_MAX
	assert(ctx.regs[s2] == UINT32_MAX); // ori s2, t0, UINT16_MAX
	assert(ctx.regs[s3] == UINT32_MAX); // ori s3, t0, 1
	                                    // addiu t0, zero, 8
	assert(ctx.regs[s4] == 9);          // ori s4, t0, 1
	assert(ctx.regs[s5] == 24);         // ori s5, t0, 16
	assert(ctx.regs[s6] == 8);          // ori s6, t0, 8
	printf("#################### ori tests finished  ######################################\n");
}

void test_xori(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### xori tests start ##########################################\n");
	ReadELF("xori.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == UINT16_MAX);              // xori s0, zero, UINT16_MAX
	assert(ctx.regs[s1] == 1);                       // xori s1, zero, 1
	                                                 // addiu t0, zero, UINT16_MAX
	assert(ctx.regs[s2] == UINT32_MAX - UINT16_MAX); // xori s2, t0, UINT16_MAX
	assert(ctx.regs[s3] == UINT32_MAX - 1);          // xori s3, t0, 1
	                                                 // addiu t0, zero, 8
	assert(ctx.regs[s4] == 9);                       // xori s4, t0, 1
	assert(ctx.regs[s5] == 24);                      // xori s5, t0, 16
	assert(ctx.regs[s6] == 0);                       // xori s6, t0, 8
	assert(ctx.regs[s7] == 17);                      // xori s7, t0, 25
	printf("#################### xori tests finished  ######################################\n");
}

void test_lui(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### lui tests start ##########################################\n");
	ReadELF("lui.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);                       // lui s0, 0
	assert(ctx.regs[s1] == UINT32_MAX - UINT16_MAX); // lui s1, UINT16_MAX
	assert(ctx.regs[s2] == 1 << 16);                 // lui s2, 1
	                                                 // addiu t0, zero, UINT16_MAX
	                                                 // ori s3, zero, UINT16_MAX
	assert(ctx.regs[s3] == UINT16_MAX << 16);        // lui s3, UINT16_MAX
	assert(ctx.regs[s4] == UINT32_MAX);              // ori s4, s3, UINT16_MAX
	printf("#################### lui tests finished  ######################################\n");
}

void test_slti(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### slti tests start ##########################################\n");
	ReadELF("slti.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	                                                 // addi t0, zero, 0
	assert(ctx.regs[s0] == 0);                       // slti s0, t0, 0
	assert(ctx.regs[s1] == 0);                       // slti s1, t0, -1
	assert(ctx.regs[s2] == 1);                       // slti s2, t0, 1
	                                                 // addi t0, zero, -1
	assert(ctx.regs[s3] == 1);                       // slti s3, t0, 0
	assert(ctx.regs[s4] == 0);                       // slti s4, t0, -1
	assert(ctx.regs[s5] == 0);                       // slti s5, t0, -2
	                                                 // addi t0, zero, INT16_MIN
	assert(ctx.regs[s6] == 1);                       // slti s6, t0, INT16_MAX
	                                                 // addi t0, zero, INT16_MAX
	assert(ctx.regs[s7] == 0);                       // slti s7, t0, INT16_MIN
	printf("#################### slti tests finished  ######################################\n");
}

int main()
{
	union mips_instruction inst;
	struct virtual_mem_region* memory = NULL;
	struct context ctx;

	test_addi(inst, memory, ctx);
	test_addiu(inst, memory, ctx);
	test_andi(inst, memory, ctx);
	test_ori(inst, memory, ctx);
	test_xori(inst, memory, ctx);
	test_lui(inst, memory, ctx);
	test_slti(inst, memory, ctx);

	printf("All tests completed successfully!\n");
	return 0;
}
