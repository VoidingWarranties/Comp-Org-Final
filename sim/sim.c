/**
	@file
	@author Andrew D. Zonenberg
	@brief The core of the simulator
 */
#include "sim.h"
#include "instructions.h"

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

	switch (inst->rtype.opcode) {
		case OP_RTYPE:
			return_val = SimulateRtypeInstruction(inst, memory, ctx);
			break;
		case OP_ADDIU:
			return_val = addiu(inst->itype.rt, inst->itype.rs, inst->itype.imm, ctx);
			break;
		case OP_LW:
			return_val = lw(inst->itype.rt, inst->itype.rs, inst->itype.imm, memory, ctx);
			break;
		case OP_SW:
			return_val = sw(inst->itype.rt, inst->itype.rs, inst->itype.imm, memory, ctx);
			break;
		case OP_LB:
			return_val = lb(inst->itype.rt, inst->itype.rs, inst->itype.imm, memory, ctx);
			break;
		case OP_SB:
			return_val = sb(inst->itype.rt, inst->itype.rs, inst->itype.imm, memory, ctx);
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

	int return_val = 1;

	switch (inst->rtype.func) {
		case FUNC_SYSCALL:
			return_val = SimulateSyscall(ctx->regs[v0], memory, ctx);
			break;
	}

	return return_val;
}

int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx)
{
	switch (callnum) {
		case SYSCALL_EXIT:
			return 0;
			break;
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
