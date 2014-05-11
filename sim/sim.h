/**
	@file
	@author Andrew D. Zonenberg
	@brief Main program include file
 */

#ifndef sim_h
#define sim_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System headers

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/elf.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Opcode table

enum opcodes
{
	OP_JAL		= 0x03,

	OP_RTYPE	= 0x00,
	OP_ADDI		= 0x08,
	OP_ADDIU	= 0x09,
	OP_SLTI		= 0x0a,
	OP_SLTIU	= 0x0b,
	OP_ANDI		= 0x0c,
	OP_ORI		= 0x0d,
	OP_XORI		= 0x0e,
	OP_LUI		= 0x0f,
	OP_LB		= 0x20,
	OP_LW		= 0x23,
	OP_SB		= 0x28,
	OP_SW		= 0x2b
};

enum functions
{
	FUNC_JR		= 0x08,

	FUNC_SLL	= 0x00,
	FUNC_SRL	= 0x02,
	FUNC_SLLV	= 0x04,
	FUNC_SRLV	= 0x06,
	FUNC_SYSCALL	= 0x0c,
	FUNC_ADD	= 0x20,
	FUNC_ADDU	= 0x21,
	FUNC_SUB	= 0x22,
	FUNC_SUBU	= 0x23,
	FUNC_AND	= 0x24,
	FUNC_OR		= 0x25,
	FUNC_XOR	= 0x26,
	FUNC_SLT	= 0x2a,
	FUNC_SLTU	= 0x2b
};

enum syscall
{
	SYSCALL_PRINT_INT	= 1,
	SYSCALL_READ_INT	= 5,
	SYSCALL_EXIT		= 10
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A MIPS instruction

//This union allows easily pulling bitfields out from instructions
union mips_instruction
{
	//R-type format
	struct
	{
		unsigned int func	: 6;
		unsigned int shamt	: 5;
		unsigned int rd		: 5;
		unsigned int rt		: 5;
		unsigned int rs		: 5;
		unsigned int opcode	: 6;
	} rtype;

	//I-type format
	struct
	{
		unsigned int imm	: 16;
		unsigned int rt		: 5;
		unsigned int rs		: 5;
		unsigned int opcode	: 6;
	} itype;

	//J-type format
	struct
	{
		unsigned int addr	: 26;
		unsigned int opcode	: 6;
	} jtype;

	//Write to this to load an instruction as a 32-bit word
	uint32_t word;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Virtual memory

/**
	@brief One contiguous region of virtual memory (corresponds to an ELF program header).
 */
struct virtual_mem_region
{
	uint32_t vaddr;
	uint32_t len;
	uint32_t* data;
	struct virtual_mem_region* next;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPU context

/**
	@brief All CPU registers
 */
struct context
{
	uint32_t pc;
	uint32_t regs[32];
};

enum mips_regids
{
	zero,
	at,
	v0, v1,
	a0, a1, a2, a3,
	t0, t1, t2, t3, t4, t5, t6, t7,
	s0, s1, s2, s3, s4, s5, s6, s7,
	t8, t9,
	k0, k1,
	gp,
	sp,
	fp,
	s8 = fp,
	ra
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Startup

void ReadELF(const char* fname, struct virtual_mem_region** memory, struct context* ctx);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simulator core

void RunSimulator(struct virtual_mem_region* memory, struct context* ctx);

uint32_t FetchWordFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory);
void StoreWordToVirtualMemory(uint32_t address, uint32_t value, struct virtual_mem_region* memory);
uint8_t FetchByteFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory);
void StoreByteToVirtualMemory(uint32_t address, uint8_t value, struct virtual_mem_region* memory);

int SimulateInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
int SimulateRtypeInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx);
int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx);

#endif
