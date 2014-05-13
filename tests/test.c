#include <stdio.h>

#include "../sim/sim.h"
#include "itype-tests.h"

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

	printf("All tests completed successfully!\n");
	return 0;
}
