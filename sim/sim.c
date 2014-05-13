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

int8_t FetchByteFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory)
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
	//Go on to next instruction by default
	//Need to change this for branches
	ctx->pc += 4;

	//TODO: Switch on opcode, if R-type instruction call SimulateRTypeInstruction()
	//otherwise it's I/J type

	unsigned int opcode = inst->rtype.opcode;

	if (opcode == OP_RTYPE) {
		return SimulateRtypeInstruction(inst, memory, ctx);
	}

	// insert instructions that don't modify rt here
	switch (opcode) {
		case OP_BEQ:
			if (ctx->regs[inst->itype.rs] == ctx->regs[inst->itype.rt]) {
				ctx->pc += (int16_t)inst->itype.imm << 2;
			}
			return 1;
		case OP_BNE:
			if (ctx->regs[inst->itype.rs] != ctx->regs[inst->itype.rt]) {
				ctx->pc +=  (int16_t)inst->itype.imm << 2;
			}
			return 1;
		case OP_BLTZ_BGEZ:
			switch (inst->itype.rt) {
				case BRANCH_BLTZ:
					if (ctx->regs[inst->itype.rs] < 0) {
						ctx->pc += (int16_t)inst->itype.imm << 2;
					}
					return 1;
				case BRANCH_BGEZ:
					if (ctx->regs[inst->itype.rs] >= 0) {
						ctx->pc += (int16_t)inst->itype.imm << 2;
					}
					return 1;
				case BRANCH_BLTZAL:
					if (ctx->regs[inst->itype.rs] < 0) {
						ctx->regs[ra] = ctx->pc + 4;
						ctx->pc += (int16_t)inst->itype.imm << 2;
					}
					return 1;
				case BRANCH_BGEZAL:
					if (ctx->regs[inst->itype.rs] >= 0) {
						ctx->regs[ra] = ctx->pc + 4;
						ctx->pc += (int16_t)inst->itype.imm << 2;
					}
					return 1;
				default:
					printf("\nUnknown branching instruction! Terminating...\n");
					return 0;
			}
		case OP_BLEZ:
			// do we need to check if rt is 0???
			if (ctx->regs[inst->itype.rs] <= 0) {
				ctx->pc += (int16_t)inst->itype.imm << 2;
			}
			return 1;
		case OP_BGTZ:
			// do we need to check if rt is 0???
			if (ctx->regs[inst->itype.rs] > 0) {
				ctx->pc += (int16_t)inst->itype.imm << 2;
			}
			return 1;
		case OP_J:
			// The upper 4 bits are the same as the upper 4 bits of the next pc.
			// The lower 28 bits are the addr shifted left by 2.
			ctx->pc = ((0b1111 << 28) & ctx->pc) | (inst->jtype.addr << 2);
			return 1;
		case OP_JAL:
			ctx->regs[ra] = ctx->pc + 4;
			// The upper 4 bits are the same as the upper 4 bits of the next pc.
			// The lower 28 bits are the addr shifted left by 2.
			ctx->pc = ((0b1111 << 28) & ctx->pc) | (inst->jtype.addr << 2);
			return 1;
	}

	if (inst->itype.rt == zero &&
	    (opcode == OP_ADDI || opcode == OP_ADDIU || opcode == OP_SLTI || opcode == OP_SLTIU ||
	     opcode == OP_ANDI || opcode == OP_ORI || opcode == OP_XORI ||
	     opcode == OP_LUI || opcode == OP_LW || opcode == OP_LB)) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	switch (opcode) {
		// I-type
		case OP_ADDI:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] + (int16_t)inst->itype.imm;
			return 1;
		case OP_ADDIU:
			// addiu is effectively the same as addi since we are not implementing overflow traps here
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] + (int16_t)inst->itype.imm;
			return 1;
		case OP_SLTI:
			ctx->regs[inst->rtype.rd] = (int32_t)ctx->regs[inst->rtype.rs] < (int16_t)inst->itype.imm ? 1 : 0;
			return 1;
		case OP_SLTIU:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] < inst->itype.imm ? 1 : 0;
			return 1;
		case OP_ANDI:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] & inst->itype.imm;
			return 1;
		case OP_ORI:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] | inst->itype.imm;
			return 1;
		case OP_XORI:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] ^ inst->itype.imm;
			return 1;

		// Memory load / store
		case OP_LUI:
			ctx->regs[inst->itype.rt] = inst->itype.imm << 16;
			return 1;
		case OP_LW:
			ctx->regs[inst->itype.rt] = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + (int16_t)inst->itype.imm, memory);
			return 1;
		case OP_SW:
			StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + (int16_t)inst->itype.imm, ctx->regs[inst->itype.rt], memory);
			return 1;
		case OP_LB:
			ctx->regs[inst->itype.rt] = (int8_t)FetchByteFromVirtualMemory(ctx->regs[inst->itype.rs] + (int16_t)inst->itype.imm, memory);
			return 1;
		case OP_SB:
			StoreByteToVirtualMemory(ctx->regs[inst->itype.rs] + (int16_t)inst->itype.imm, ctx->regs[inst->itype.rt], memory);
			return 1;

		default:
			printf("\nUnknown instruction! Terminating...\n");
			return 0;
	}
}

int SimulateRtypeInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	//TODO: switch on func, if syscall call SimulateSyscall()
	//else process instruction normally

	if (inst->rtype.func == FUNC_SYSCALL) {
		return SimulateSyscall(ctx->regs[v0], memory, ctx);
	}
	// instructions that do not modify rd
	switch (inst->rtype.func) {
		case FUNC_MULT: {
			int64_t result = (int64_t)((int32_t)ctx->regs[inst->rtype.rs]) * (int64_t)((int32_t)ctx->regs[inst->rtype.rt]);
			ctx->LO = (result << 32) >> 32;
			ctx->HI = result >> 32;
			return 1;
		}
		case FUNC_MULTU: {
			uint64_t result = (uint64_t)ctx->regs[inst->rtype.rs] * (uint64_t)ctx->regs[inst->rtype.rt];
			ctx->LO = (result << 32) >> 32;
			ctx->HI = result >> 32;
			return 1;
		}
		case FUNC_DIV:
			ctx->LO = (int32_t)ctx->regs[inst->rtype.rs] / (int32_t)ctx->regs[inst->rtype.rt];
			ctx->HI = (int32_t)ctx->regs[inst->rtype.rs] % (int32_t)ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_DIVU:
			ctx->LO = ctx->regs[inst->rtype.rs] / ctx->regs[inst->rtype.rt];
			ctx->HI = ctx->regs[inst->rtype.rs] % ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_JR:
			ctx->pc = ctx->regs[inst->rtype.rs];
			return 1;
	}
	if (inst->rtype.rd == zero) {
		printf("\nCannot modify $zero register! Terminating...\n");
		return 0;
	}
	// instructions that do modify rd
	switch (inst->rtype.func) {
		case FUNC_ADD:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_ADDU:
			// addu is effectively the same as add since we are not implementing overflow traps here
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_SUB:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_SUBU:
			// subu is effectively the same as sub since we are not implementing overflow traps here
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_AND:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] & ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_OR:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] | ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_XOR:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] ^ ctx->regs[inst->rtype.rt];
			return 1;
		case FUNC_SLT:
			// test this extensively to make sure it makes signed comparisons correctly
			ctx->regs[inst->rtype.rd] = (int32_t)ctx->regs[inst->rtype.rs] < (int32_t)ctx->regs[inst->rtype.rt] ? 1 : 0;
			return 1;
		case FUNC_SLTU:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] < ctx->regs[inst->rtype.rt] ? 1 : 0;
			return 1;
		case FUNC_SLL:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] << inst->rtype.shamt;
			return 1;
		case FUNC_SRL:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] >> inst->rtype.shamt;
			return 1;
		case FUNC_SLLV:
			// The official MIPS specification states that rt should be shifted by the amount specified
			// by the 5 least significant bits of rs. This info doesn't seem to be in the textbook, but
			// I trust the official MIPS specification more than anything else.
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] << (ctx->regs[inst->rtype.rs] & 0b11111);
			return 1;
		case FUNC_SRLV:
			// The official MIPS specification states that rt should be shifted by the amount specified
			// by the 5 least significant bits of rs. This info doesn't seem to be in the textbook, but
			// I trust the official MIPS specification more than anything else.
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] >> (ctx->regs[inst->rtype.rs] & 0b11111);
			return 1;
		case FUNC_SRA:
			ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] >> inst->rtype.shamt;
			if (ctx->regs[inst->rtype.rt] >> 31) {
				for (size_t i = 0; i < inst->rtype.shamt; ++i) {
					ctx->regs[inst->rtype.rd] |= (1 << (31 - i));
				}
			}
			return 1;
		case FUNC_MFHI:
			ctx->regs[inst->rtype.rd] = ctx->HI;
			break;
		case FUNC_MFLO:
			ctx->regs[inst->rtype.rd] = ctx->LO;
			break;
		default:
			printf("\nUnknown R-type instruction! Terminating...\n");
			return 0;
	}
}

int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx)
{
	switch (callnum) {
		case SYSCALL_EXIT:
			return 0;
		case SYSCALL_PRINT_INT:
			printf("%d", ctx->regs[a0]);
			return 1;
		case SYSCALL_PRINT_STR: {
			uint32_t address = ctx->regs[a0];
			char c = FetchByteFromVirtualMemory(address, memory);
			while (c != '\0') {
				printf("%c", c);
				c = FetchByteFromVirtualMemory(address, memory);
				address += 1;
			}
			return 1;
		}
		case SYSCALL_READ_INT:
			if (scanf("%d", &ctx->regs[v0]) == EOF) {
				printf("\nUnable to read stdin. Temrinating...\n");
				return 0;
			}
			return 1;
		case SYSCALL_READ_STR: {
			char* str = (char*)calloc(ctx->regs[a1], sizeof(char));
			if (fgets(str, ctx->regs[a1], stdin) == NULL) {
				printf("\nUnable to read stdin. Terminating...\n");
				return 0;
			}
			char c;
			size_t i = 0;
			do {
				c = str[i];
				StoreByteToVirtualMemory(ctx->regs[a0] + i, c, memory);
				++i;
			} while(c != '\0' && i < ctx->regs[a1]);
			free(str);
			return 1;
		}
		case SYSCALL_SBRK: {
			struct virtual_mem_region* region = (struct virtual_mem_region*)calloc(sizeof(struct virtual_mem_region), 1);
			region->vaddr = memory->vaddr;
			region->len = memory->len;
			region->data = memory->data;
			region->next = memory->next;
			memory->vaddr = region->vaddr + region->len;
			memory->len = ctx->regs[a0];
			memory->data = calloc(memory->len, 1);
			memory->next = region;
			return 1;
		}
		default:
			printf("\nUnknown syscall instruction! Terminating...\n");
			return 0;
	}
}
