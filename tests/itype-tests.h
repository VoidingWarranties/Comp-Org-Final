void test_addi(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
void test_addiu(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
void test_andi(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
void test_ori(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
void test_xori(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
void test_lui(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
void test_slti(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
void test_sltiu(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
void test_sw(union mips_instruction inst, struct virtual_mem_region* memory, struct context ctx);
