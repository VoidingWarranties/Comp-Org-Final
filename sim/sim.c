/**
	@file
	@author Andrew D. Zonenberg
	@brief The core of the simulator
 */
#include "sim.h"

/**
	@brief Reads an ELF executable
 */
void ReadELF(const char* fname, struct virtual_mem_region** memory, struct context* ctx)
{
	//Zeroize all context stuff
	for(int i=0; i<32; i++)
		ctx->regs[i] = 0;

	//Open the file
	printf("Reading ELF file %s...\n", fname);
	FILE* fp = fopen(fname, "rb");
	if(fp == NULL)
	{
		printf("failed to load\n");
		exit(1);
	}

	//Read and validate the ELF header
	Elf32_Ehdr hdr;
	if(1 != fread(&hdr, sizeof(hdr), 1, fp))
	{
		printf("failed to read elf header\n");
		exit(1);
	}
	if( (hdr.e_ident[EI_MAG0] != 0x7f) ||
	    (hdr.e_ident[EI_MAG1] != 'E') ||
	    (hdr.e_ident[EI_MAG2] != 'L') ||
	    (hdr.e_ident[EI_MAG3] != 'F') )
	{
		printf("bad ELF magic\n");
		exit(1);
	}
	if(hdr.e_ident[EI_DATA] != ELFDATA2LSB)
	{
		printf("not little endian\n");
		exit(1);
	}
	if(hdr.e_ident[EI_CLASS] != ELFCLASS32)
	{
		printf("not an ELFCLASS32\n");
		exit(1);
	}
	if(hdr.e_machine != EM_MIPS)
	{
		printf("not a MIPS binary\n");
		exit(1);
	}
	if(hdr.e_type != ET_EXEC)
	{
		printf("not an executable file\n");
		exit(1);
	}
	if(hdr.e_version != EV_CURRENT)
	{
		printf("not the right ELF version\n");
		exit(1);
	}

	//Save the entry point address
	printf("    Virtual address of entry point is 0x%08x\n", hdr.e_entry);
	ctx->pc = hdr.e_entry;

	//Read the program headers
	if(0 != fseek(fp, hdr.e_phoff, SEEK_SET))
	{
		printf("fail to seek to phdrs\n");
		exit(1);
	}
	if(hdr.e_phentsize != sizeof(Elf32_Phdr))
	{
		printf("invalid phentsize\n");
		exit(1);
	}
	for(size_t i=0; i<hdr.e_phnum; i++)
	{
		Elf32_Phdr phdr;
		if(1 != fread(&phdr, sizeof(phdr), 1, fp))
		{
			printf("fail to read phdr\n");
			exit(1);
		}

		//Make the memory segment
		struct virtual_mem_region* region = (struct virtual_mem_region*)calloc(sizeof(struct virtual_mem_region), 1);
		region->vaddr = phdr.p_vaddr;
		region->len = phdr.p_memsz;
		region->data = calloc(phdr.p_memsz, 1);
		region->next = *memory;
		*memory = region;
		printf("    Mapping %5d bytes of virtual memory from executable at address 0x%08x\n", region->len, region->vaddr);

		//Skip non-loadable stuff
		if(phdr.p_type != PT_LOAD)
		{
			continue;
		}

		//Get ready to read the segment
		long current_pos = ftell(fp);
		if(0 != fseek(fp, phdr.p_offset, SEEK_SET))
		{
			printf("fail to re-seek\n");
			exit(1);
		}

		//Read the memory segment
		if(phdr.p_filesz != 0)
		{
			if(phdr.p_filesz != fread(region->data, 1, phdr.p_filesz, fp))
			{
				printf("fail to read memory region\n");
				exit(1);
			}
		}

		//Done, go back to where we were
		if(0 != fseek(fp, current_pos, SEEK_SET))
		{
			printf("fail to re-seek\n");
			exit(1);
		}
	}

	fclose(fp);

	//Create one last memory region for the stack, then point the stack pointer to it
	struct virtual_mem_region* region = (struct virtual_mem_region*)calloc(sizeof(struct virtual_mem_region), 1);
	region->vaddr = 0xc0000000;
	region->len = 0x8000;
	region->data = calloc(region->len, 1);
	region->next = *memory;
	*memory = region;
	ctx->regs[sp] = region->vaddr + region->len - 4;
	printf("    Mapping %d bytes of virtual memory for stack at address 0x%08x\n", region->len, region->vaddr);
}

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
					if ((int32_t)ctx->regs[inst->itype.rs] < 0) {
						ctx->pc += (int16_t)inst->itype.imm << 2;
					}
					return 1;
				case BRANCH_BGEZ:
					if ((int32_t)ctx->regs[inst->itype.rs] >= 0) {
						ctx->pc += (int16_t)inst->itype.imm << 2;
					}
					return 1;
				case BRANCH_BLTZAL:
					if ((int32_t)ctx->regs[inst->itype.rs] < 0) {
						ctx->regs[ra] = ctx->pc + 4;
						ctx->pc += (int16_t)inst->itype.imm << 2;
					}
					return 1;
				case BRANCH_BGEZAL:
					if ((int32_t)ctx->regs[inst->itype.rs] >= 0) {
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
			ctx->regs[inst->itype.rt] = (int32_t)ctx->regs[inst->rtype.rs] < (int16_t)inst->itype.imm ? 1 : 0;
			return 1;
		case OP_SLTIU:
			ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] < inst->itype.imm ? 1 : 0;
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
			// Note that the retrieved byte is sign extended to 32 bits.
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
				address += 1;
				c = FetchByteFromVirtualMemory(address, memory);
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
		// I don't think we were required to implement this, but here it is anyways :)
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
