#ifndef instructions_h
#define instructions_h

#include "sim.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I-type

int addiu(uint32_t rt, uint32_t rs, uint32_t imm, struct context* ctx);
int lw(uint32_t rt, uint32_t rs, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int sw(uint32_t rt, uint32_t rs, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int lb(uint32_t rt, uint32_t rs, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int sb(uint32_t rt, uint32_t rs, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);

#endif
