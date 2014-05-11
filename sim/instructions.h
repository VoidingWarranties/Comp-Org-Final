#ifndef instructions_h
#define instructions_h

#include "sim.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R-type

int MIPS_add(uint32_t rs, uint32_t rt, uint32_t rd, struct context* ctx);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I-type

int MIPS_addi(uint32_t rs, uint32_t rt, uint32_t imm, struct context* ctx);
int MIPS_andi(uint32_t rs, uint32_t rt, uint32_t imm, struct context* ctx);
int MIPS_ori(uint32_t rs, uint32_t rt, uint32_t imm, struct context* ctx);
int MIPS_xori(uint32_t rs, uint32_t rt, uint32_t imm, struct context* ctx);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory load / store

int MIPS_lui(uint32_t rt, uint32_t imm, struct context* ctx);
int MIPS_lw(uint32_t rs, uint32_t rt, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int MIPS_sw(uint32_t rs, uint32_t rt, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int MIPS_lb(uint32_t rs, uint32_t rt, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int MIPS_sb(uint32_t rs, uint32_t rt, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);

#endif
