#include <assert.h>

#include "../sim/sim.h"

void test_addi(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### addi tests start ##########################################\n");
	ReadELF("MIPS_tests/addi.elf", &memory, &ctx);
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
	ReadELF("MIPS_tests/addiu.elf", &memory, &ctx);
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
	ReadELF("MIPS_tests/andi.elf", &memory, &ctx);
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
	ReadELF("MIPS_tests/ori.elf", &memory, &ctx);
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
	ReadELF("MIPS_tests/xori.elf", &memory, &ctx);
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
	ReadELF("MIPS_tests/lui.elf", &memory, &ctx);
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
	ReadELF("MIPS_tests/slti.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	                           // addi t0, zero, 0
	assert(ctx.regs[s0] == 0); // slti s0, t0, 0
	assert(ctx.regs[s1] == 0); // slti s1, t0, -1
	assert(ctx.regs[s2] == 1); // slti s2, t0, 1
	                           // addi t0, zero, -1
	assert(ctx.regs[s3] == 1); // slti s3, t0, 0
	assert(ctx.regs[s4] == 0); // slti s4, t0, -1
	assert(ctx.regs[s5] == 0); // slti s5, t0, -2
	                           // addi t0, zero, INT16_MIN
	assert(ctx.regs[s6] == 1); // slti s6, t0, INT16_MAX
	                           // addi t0, zero, INT16_MAX
	assert(ctx.regs[s7] == 0); // slti s7, t0, INT16_MIN
	printf("#################### slti tests finished  ######################################\n");
}

void test_sltiu(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### sltiu tests start ##########################################\n");
	ReadELF("MIPS_tests/sltiu.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	                           // addi t0, zero, 0
	assert(ctx.regs[s0] == 0); // sltiu s0, t0, 0
	assert(ctx.regs[s1] == 1); // sltiu s1, t0, -1
	assert(ctx.regs[s2] == 1); // sltiu s2, t0, 1
	                           // addi t0, zero, -1
	assert(ctx.regs[s3] == 0); // sltiu s3, t0, 0
	assert(ctx.regs[s4] == 0); // sltiu s4, t0, -1
	assert(ctx.regs[s5] == 0); // sltiu s5, t0, -2
	                           // addi t0, zero, 0
	assert(ctx.regs[s6] == 1); // sltiu s6, t0, UINT16_MAX
	                           // addi t0, zero, UINT16_MAX
	assert(ctx.regs[s7] == 0); // sltiu s7, t0, 0
	printf("#################### sltiu tests finished  ######################################\n");
}

void test_sw(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### sw tests start ##########################################\n");
	ReadELF("MIPS_tests/sw.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	size_t last_index = (memory->len / 4) - 1;

	                                            // addi t0, zero, 0
	assert(memory->data[last_index - 0] == 0);  // sw t0, 0(sp)
	                                            // addi t0, zero, 1
	assert(memory->data[last_index - 1] == 1);  // sw t0, -4(sp)
	                                            // addi t0, zero, 2
	assert(memory->data[last_index - 2] == 2);  // sw t0, -8(sp)
	                                            // addi t0, zero, -1
	assert(memory->data[last_index - 3] == -1); // sw t0, -12(sp)
	printf("#################### sw tests finished  ######################################\n");
}

void test_lw(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### lw tests start ##########################################\n");
	ReadELF("MIPS_tests/lw.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	                            // addi t0, zero, 0
	                            // sw t0, 0(sp)
	assert(ctx.regs[s0] == 0);  // lw s0, 0(sp)
	                            // addi t0, zero, 1
	                            // sw t0, -4(sp)
	assert(ctx.regs[s1] == 1);  // lw s1, -4(sp)
	                            // addi t0, zero, 2
	                            // sw t0, -8(sp)
	assert(ctx.regs[s2] == 2);  // lw s2, -8(sp)
				    // addi t0, zero, -1
	                            // sw t0, -12(sp)
	assert(ctx.regs[s3] == -1); // lw s3, -12(sp)
	printf("#################### lw tests finished  ######################################\n");
}

void test_sb(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### sb tests start ##########################################\n");
	ReadELF("MIPS_tests/sb.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	uint8_t* byte_data = (uint8_t*)(memory->data);
	size_t last_index = memory->len - 4;

	                                          // addi t0, zero, 0
	assert(byte_data[last_index - 0] == 0);   // sb t0, 0(sp)
	                                          // addi t0, zero, 1
	assert(byte_data[last_index - 1] == 1);   // sb t0, -1(sp)
	                                          // addi t0, zero, 2
	assert(byte_data[last_index - 2] == 2);   // sb t0, -2(sp)
	                                          // addi t0, zero, -1
	assert(byte_data[last_index - 3] == 255); // sb t0, -3(sp)
	printf("#################### sb tests finished  ######################################\n");
}

void test_lb(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### lb tests start ##########################################\n");
	ReadELF("MIPS_tests/lb.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	                            // addi t0, zero, 0
	                            // sb t0, 0(sp)
	assert(ctx.regs[s0] == 0);  // lb s0, 0(sp)
	                            // addi t0, zero, 1
	                            // sb t0, -1(sp)
	assert(ctx.regs[s1] == 1);  // lb s1, -1(sp)
	                            // addi t0, zero, 2
	                            // sb t0, -2(sp)
	assert(ctx.regs[s2] == 2);  // lb s2, -2(sp)
	                            // addi t0, zero, -1
	                            // sb t0, -3(sp)
	assert(ctx.regs[s3] == -1); // lb s3, -3(sp)
	printf("#################### lb tests finished  ######################################\n");
}

void test_beq(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### beq tests start ##########################################\n");
	ReadELF("MIPS_tests/beq.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 1); // tests if 0 == 0
	assert(ctx.regs[s1] == 0); // tests if 0 == 1
	assert(ctx.regs[s2] == 1); // tests if 0b1...1 == 0b1...1
	printf("#################### beq tests finished  ######################################\n");
}

void test_bne(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx)
{
	printf("#################### bne tests start ##########################################\n");
	ReadELF("MIPS_tests/bne.elf", &memory, &ctx);
	RunSimulator(memory, &ctx);

	assert(ctx.regs[s0] == 0); // tests if 0 != 0
	assert(ctx.regs[s1] == 1); // tests if 0 != 1
	assert(ctx.regs[s2] == 0); // tests if 0b1...1 != 0b1...1
	printf("#################### bne tests finished  ######################################\n");
}
