#ifndef instructions_h
#define instructions_h

#include "sim.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R-type

int add(uint32_t rs, uint32_t rt, uint32_t rd, struct context* ctx);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I-type

int addi(uint32_t rs, uint32_t rt, uint32_t imm, struct context* ctx);
int andi(uint32_t rs, uint32_t rt, uint32_t imm, struct context* ctx);
int ori(uint32_t rs, uint32_t rt, uint32_t imm, struct context* ctx);
int xori(uint32_t rs, uint32_t rt, uint32_t imm, struct context* ctx);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory load / store

int lui(uint32_t rt, uint32_t imm, struct context* ctx);
int lw(uint32_t rs, uint32_t rt, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int sw(uint32_t rs, uint32_t rt, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int lb(uint32_t rs, uint32_t rt, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);
int sb(uint32_t rs, uint32_t rt, uint32_t imm, struct virtual_mem_region* memory, struct context* ctx);

#endif
