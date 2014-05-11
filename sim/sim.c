/**
	@file
	@author Andrew D. Zonenberg
	@brief The core of the simulator
 */
#include "sim.h"

/**
	@brief Read logic for instruction fetch and load instructions
	
	Address must be aligned
 */
uint32_t FetchWordFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory)
{
	//Traverse the linked list until we find the range of interest
	while(memory != NULL)
	{
		//Not in range? Try next one
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) )
		{
			memory = memory->next;
			continue;
		}

		//Align check
		uint32_t offset = address - memory->vaddr;
		if(offset & 3)
		{
			printf("SEGFAULT: address %08x is not aligned\n", address);
			exit(1);
		}

		return memory->data[offset/4];
	}

	//Didn't find anything! Give up
	printf("SEGFAULT: attempted to read word from nonexistent virtual address %08x\n", address);
	exit(1);
}

/**
	@brief Write logic for store instructions.

	Stores an entire 32-bit word. sh/sb instructions will need to do a read-modify-write structure
 */
void StoreWordToVirtualMemory(uint32_t address, uint32_t value, struct virtual_mem_region* memory)
{
	//Traverse the linked list until we find the range of interest
	while(memory != NULL)
	{
		//Not in range? Try next one
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) )
		{
			memory = memory->next;
			continue;
		}

		//Align check
		uint32_t offset = address - memory->vaddr;
		if(offset & 3)
		{
			printf("SEGFAULT: address %08x is not aligned\n", address);
			exit(1);
		}

		memory->data[offset/4] = value;
		return;
	}

	//Didn't find anything! Give up
	printf("SEGFAULT: attempted to write word to nonexistent virtual address %08x\n", address);
	exit(1);
}

uint8_t FetchByteFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory)
{
	//Traverse the linked list until we find the range of interest
	while(memory != NULL)
	{
		//Not in range? Try next one
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) )
		{
			memory = memory->next;
			continue;
		}

		uint32_t offset = address - memory->vaddr;
		uint32_t byte_offset = 8 * (offset % 4);
		uint32_t word = memory->data[offset/4];
		uint8_t byte = (word >> byte_offset) & 255;
		return byte;
	}

	//Didn't find anything! Give up
	printf("SEGFAULT: attempted to read word from nonexistent virtual address %08x\n", address);
	exit(1);
}

void StoreByteToVirtualMemory(uint32_t address, uint8_t value, struct virtual_mem_region* memory)
{
	//Traverse the linked list until we find the range of interest
	while(memory != NULL)
	{
		//Not in range? Try next one
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) )
		{
			memory = memory->next;
			continue;
		}

		uint32_t offset = address - memory->vaddr;
		uint32_t byte_offset = 8 * (offset % 4);
		uint32_t word = memory->data[offset/4];
		word = word | (255 << byte_offset) & (value << byte_offset);
		memory->data[offset/4] = word;
		return;
	}

	//Didn't find anything! Give up
	printf("SEGFAULT: attempted to read word from nonexistent virtual address %08x\n", address);
	exit(1);
}

/**
	@brief Runs the actual simulation
 */
void RunSimulator(struct virtual_mem_region* memory, struct context* ctx)
{
	printf("Starting simulation...\n");

	union mips_instruction inst;
	while(1)
	{
		inst.word = FetchWordFromVirtualMemory(ctx->pc, memory);
		if(!SimulateInstruction(&inst, memory, ctx))
		{
			break;
		}
	}
}

/**
	@brief Simulates a single instruction

	Return 0 to exit the program (for syscall/invalid instruction) and 1 to keep going
 */
int SimulateInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	//TODO: Switch on opcode, if R-type instruction call SimulateRTypeInstruction()
	//otherwise it's I/J type

	int return_val = 1;
	unsigned int opcode = inst->rtype.opcode;

	if (opcode == OP_RTYPE) {
		return_val = SimulateRtypeInstruction(inst, memory, ctx);
	}

	// insert instructions that don't modify rt here

	if (inst->itype.rt == zero &&
	    (opcode == OP_ADDI || opcode == OP_ADDIU || opcode == OP_ANDI || opcode == OP_ORI || opcode == OP_XORI ||
	     opcode == OP_LUI || opcode == OP_LW || opcode == OP_LB)) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	switch (opcode) {
		// I-type
		case OP_ADDI:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] + inst->itype.imm;
			break;
		case OP_ADDIU:
			// addiu is effectively the same as addi since we are not implementing overflow traps here
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] + inst->itype.imm;
			break;
		case OP_ANDI:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] & inst->itype.imm;
			break;
		case OP_ORI:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] | inst->itype.imm;
			break;
		case OP_XORI:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] ^ inst->itype.imm;
			break;

		// Memory load / store
		case OP_LUI:
			ctx->regs[inst->itype.rt] = inst->itype.imm << 16;
			break;
		case OP_LW:
			ctx->regs[inst->itype.rt] = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, memory);
			break;
		case OP_SW:
			StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, ctx->regs[inst->itype.rt], memory);
			break;
		case OP_LB:
			ctx->regs[inst->itype.rt] = FetchByteFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, memory);
			break;
		case OP_SB:
			StoreByteToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, ctx->regs[inst->itype.rt], memory);
			break;
	}

	//Go on to next instruction by default
	//Need to change this for branches
	ctx->pc += 4;

	return return_val;
}

int SimulateRtypeInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	//TODO: switch on func, if syscall call SimulateSyscall()
	//else process instruction normally

	if (inst->rtype.func == FUNC_SYSCALL) {
		return SimulateSyscall(ctx->regs[v0], memory, ctx);
	}
	if (inst->rtype.rd == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	switch (inst->rtype.func) {
		case FUNC_ADD:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
			break;
		case FUNC_ADDU:
			// addu is effectively the same as add since we are not implementing overflow traps here
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
			break;
		case FUNC_SUB:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
			break;
		case FUNC_SUBU:
			// subu is effectively the same as sub since we are not implementing overflow traps here
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
			break;
		case FUNC_AND:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] & ctx->regs[inst->rtype.rt];
			break;
		case FUNC_OR:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] | ctx->regs[inst->rtype.rt];
			break;
		case FUNC_XOR:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] ^ ctx->regs[inst->rtype.rt];
			break;
		case FUNC_SLT:
			// test this extensively to make sure it makes signed comparisons correctly
			ctx->regs[inst->rtype.rd] = (int32_t)ctx->regs[inst->rtype.rs] < (int32_t)ctx->regs[inst->rtype.rt] ? 1 : 0;
		case FUNC_SLTU:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] < ctx->regs[inst->rtype.rt] ? 1 : 0;
	}

	return 1;
}

int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx)
{
	switch (callnum) {
		case SYSCALL_EXIT:
			return 0;
		case SYSCALL_PRINT_INT:
			printf("%d", ctx->regs[a0]);
			break;
		case SYSCALL_READ_INT:
			if (scanf("%d", &ctx->regs[v0]) == EOF) {
				printf("\nUnable to read stdin. Temrinating...\n");
				return 0;
			}
			break;
	}

	return 1;
}
