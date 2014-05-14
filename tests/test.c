#include <stdio.h>

#include "../sim/sim.h"
#include "itype-tests.h"
#include "rtype-tests.h"

int main()
{
	union mips_instruction inst;
	struct virtual_mem_region* memory = NULL;
	struct context ctx;

	// I-type
	test_addi(inst, memory, ctx);
	test_addiu(inst, memory, ctx);
	test_andi(inst, memory, ctx);
	test_ori(inst, memory, ctx);
	test_xori(inst, memory, ctx);
	test_lui(inst, memory, ctx);
	test_slti(inst, memory, ctx);
	test_sltiu(inst, memory, ctx);
	test_sw(inst, memory, ctx);
	test_lw(inst, memory, ctx);
	test_sb(inst, memory, ctx);
	test_lb(inst, memory, ctx);
	test_beq(inst, memory, ctx);
	test_bne(inst, memory, ctx);
	test_bgez(inst, memory, ctx);
	test_bgtz(inst, memory, ctx);
	test_blez(inst, memory, ctx);
	test_bltz(inst, memory, ctx);

	// R-type
	test_add(inst, memory, ctx);
	test_addu(inst, memory, ctx);
	test_sub(inst, memory, ctx);
	test_subu(inst, memory, ctx);
	test_and(inst, memory, ctx);
	test_or(inst, memory, ctx);
	test_xor(inst, memory, ctx);

	printf("\nAll tests completed successfully!\n");
	return 0;
}
