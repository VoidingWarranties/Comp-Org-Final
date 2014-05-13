#include <stdio.h>
#include <stdint.h>

#include "../sim/sim.h"

void test_add(union mips_instruction inst, struct context ctx)
{
	inst.rtype.opcode = OP_RTYPE;
	inst.rtype.func = FUNC_ADD;
	inst.rtype.rs = t0;
	inst.rtype.rt = t1;
	inst.rtype.rd = t2;

	size_t num_tests = 10;
	uint32_t t0_tests[] = {INT32_MIN,	INT32_MIN,	INT32_MIN,	0,			-1,			-1,	1,	-1,	0,	0};
	uint32_t t1_tests[] = {0,			1,			INT32_MAX,	INT32_MAX,	INT32_MAX,	-1,	1,	1,	-1,	0};

	printf("Testing add:\t");
	for (size_t i = 0; i < num_tests; ++i) {
		ctx.regs[inst.rtype.rs] = t0_tests[i];
		ctx.regs[inst.rtype.rt] = t1_tests[i];
		if (SimulateInstruction(&inst, NULL, &ctx) == 0 || ctx.regs[inst.rtype.rd] != t0_tests[i] + t1_tests[i]) {
			printf("\n  test failed: %d != %d + %d", ctx.regs[inst.rtype.rd], t0_tests[i], t1_tests[i]);
			exit(1);
		}
		printf(".");
	}
	printf("\tsuccess!\n");

	inst.rtype.func = FUNC_ADDU;
	printf("Testing addu:\t");
	for (size_t i = 0; i < num_tests; ++i) {
		ctx.regs[inst.rtype.rs] = t0_tests[i];
		ctx.regs[inst.rtype.rt] = t1_tests[i];
		if (SimulateInstruction(&inst, NULL, &ctx) == 0 || ctx.regs[inst.rtype.rd] != t0_tests[i] + t1_tests[i]) {
			printf("\n  test failed: %d != %d + %d", ctx.regs[inst.rtype.rd], t0_tests[i], t1_tests[i]);
			exit(1);
		}
		printf(".");
	}
	printf("\tsuccess!\n");
}

void test_sub(union mips_instruction inst, struct context ctx)
{
	inst.rtype.opcode = OP_RTYPE;
	inst.rtype.func = FUNC_SUB;
	inst.rtype.rs = t0;
	inst.rtype.rt = t1;
	inst.rtype.rd = t2;

	size_t num_tests = 13;
	uint32_t t0_tests[] = {INT32_MIN, INT32_MIN, INT32_MIN, 0,         1,         0, 1,  -1, -1, 1, INT32_MAX, INT32_MAX, INT32_MAX};
	uint32_t t1_tests[] = {0,         -1,        INT32_MIN, INT32_MIN, INT32_MIN, 0, -1, 1,  -1, 1, 0,         1,         INT32_MAX};

	printf("Testing sub:\t");
	for (size_t i = 0; i < num_tests; ++i) {
		ctx.regs[inst.rtype.rs] = t0_tests[i];
		ctx.regs[inst.rtype.rt] = t1_tests[i];
		if (SimulateInstruction(&inst, NULL, &ctx) == 0 || ctx.regs[inst.rtype.rd] != t0_tests[i] - t1_tests[i]) {
			printf("\n  test failed: %d != %d + %d", ctx.regs[inst.rtype.rd], t0_tests[i], t1_tests[i]);
			exit(1);
		}
		printf(".");
	}
	printf("\tsuccess!\n");

	inst.rtype.func = FUNC_SUBU;
	printf("Testing subu:\t");
	for (size_t i = 0; i < num_tests; ++i) {
		ctx.regs[inst.rtype.rs] = t0_tests[i];
		ctx.regs[inst.rtype.rt] = t1_tests[i];
		if (SimulateInstruction(&inst, NULL, &ctx) == 0 || ctx.regs[inst.rtype.rd] != t0_tests[i] - t1_tests[i]) {
			printf("\n  test failed: %d != %d + %d", ctx.regs[inst.rtype.rd], t0_tests[i], t1_tests[i]);
			exit(1);
		}
		printf(".");
	}
	printf("\tsuccess!\n");
}

int main()
{
	struct context ctx;
	for (size_t i = 0; i < 32; ++i) {
		ctx.regs[i] = 0;
	}
	union mips_instruction inst;

	test_add(inst, ctx);
	test_sub(inst, ctx);

	printf("All tests completed successfully!\n");
	return 0;
}
