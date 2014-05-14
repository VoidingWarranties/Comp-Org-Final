#include <assert.h>

#include "../sim/sim.h"

void test_add(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### add tests start ##########################################\n");
	ReadELF("MIPS_tests/add.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);  // add s0, zero, zero
	                            // addi t0, zero, 1
	assert(ctx.regs[s1] == 1);  // add s1, zero, t0
	                            // addi t0, zero, -1
	assert(ctx.regs[s2] == -1); // add s2, zero, t0
	assert(ctx.regs[s3] == 0);  // add s3, s2, s1
	printf("#################### add tests finished  ######################################\n");
}

void test_addu(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### addu tests start ##########################################\n");
	ReadELF("MIPS_tests/addu.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);  // addu s0, zero, zero
	                            // addi t0, zero, 1
	assert(ctx.regs[s1] == 1);  // addu s1, zero, t0
	                            // addi t0, zero, -1
	assert(ctx.regs[s2] == -1); // addu s2, zero, t0
	assert(ctx.regs[s3] == 0);  // addu s3, s2, s1
	printf("#################### addu tests finished  ######################################\n");
}

void test_sub(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### sub tests start ##########################################\n");
	ReadELF("MIPS_tests/sub.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);  // sub s0, zero, zero
	                            // addi t0, zero, 1
	assert(ctx.regs[s1] == -1); // sub s1, zero, t0
	                            // addi t0, zero, -1
	assert(ctx.regs[s2] == 1);  // sub s2, zero, t0
	assert(ctx.regs[s3] == 2);  // sub s3, s2, s1
	assert(ctx.regs[s4] == -2); // sub s3, s2, s1
	printf("#################### sub tests finished  ######################################\n");
}

void test_subu(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### subu tests start ##########################################\n");
	ReadELF("MIPS_tests/subu.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0);  // subu s0, zero, zero
	                            // addi t0, zero, 1
	assert(ctx.regs[s1] == -1); // subu s1, zero, t0
	                            // addi t0, zero, -1
	assert(ctx.regs[s2] == 1);  // subu s2, zero, t0
	assert(ctx.regs[s3] == 2);  // subu s3, s2, s1
	assert(ctx.regs[s4] == -2); // subu s3, s2, s1
	printf("#################### subu tests finished  ######################################\n");
}

void test_and(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### and tests start ##########################################\n");
	ReadELF("MIPS_tests/and.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	                            // addi t0, zero, 1
	                            // addi t1, zero, UINT16_MAX
	assert(ctx.regs[s0] == 0);  // and s0, zero, zero
	assert(ctx.regs[s1] == 0);  // and s1, zero, t0
	assert(ctx.regs[s2] == 0);  // and s2, zero, t1
	assert(ctx.regs[s3] == 1);  // and s3, t0, t0
	assert(ctx.regs[s4] == -1); // and s4, t1, t1
	                            // addi t2, zero, 5
	assert(ctx.regs[s5] == 1);  // and s5, t0, t2
	assert(ctx.regs[s6] == 5);  // and s6, t1, t2
	printf("#################### and tests finished  ######################################\n");
}

void test_or(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### or tests start ##########################################\n");
	ReadELF("MIPS_tests/or.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	                            // addi t0, zero, 1
	                            // addi t1, zero, UINT16_MAX
	assert(ctx.regs[s0] == 0);  // or s0, zero, zero
	assert(ctx.regs[s1] == 1);  // or s1, zero, t0
	assert(ctx.regs[s2] == -1); // or s2, zero, t1
	assert(ctx.regs[s3] == 1);  // or s3, t0, t0
	assert(ctx.regs[s4] == -1); // or s4, t1, t1
	                            // addi t1, zero, 8
	assert(ctx.regs[s5] == 9);  // or s5, t1, t0
	                            // addi t0, zero, 16
	assert(ctx.regs[s6] == 24); // or s6, t1, t0
	                            // addi t0, zero, 8
	assert(ctx.regs[s7] == 8);  // or s7, t1, t0
	printf("#################### or tests finished  ######################################\n");
}

void test_xor(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### xor tests start ##########################################\n");
	ReadELF("MIPS_tests/xor.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	                            // addi t0, zero, 1
	                            // addi t1, zero, UINT16_MAX
	assert(ctx.regs[s0] == 0);  // xor s0, zero, zero
	assert(ctx.regs[s1] == 1);  // xor s1, zero, t0
	assert(ctx.regs[s2] == -1); // xor s2, zero, t1
	assert(ctx.regs[s3] == 0);  // xor s3, t0, t0
	assert(ctx.regs[s4] == 0);  // xor s4, t1, t1
	                            // addi t1, zero, 8
	assert(ctx.regs[s5] == 9);  // xor s5, t1, t0
	                            // addi t0, zero, 16
	assert(ctx.regs[s6] == 24); // xor s6, t1, t0
	                            // addi t0, zero, 8
	assert(ctx.regs[s7] == 0);  // xor s7, t1, t0
	                            // addi t0, zero, 25
	assert(ctx.regs[s8] == 17); // xor s8, t1, t0
	printf("#################### xor tests finished  ######################################\n");
}
