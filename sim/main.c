/**
	@file
	@author Andrew D. Zonenberg
	@brief Startup code for simulator
 */
#include "sim.h"

/**
	@brief Program entry point
 */
int main(int argc, char* argv[])
{
	//Sanity check args
	if(argc != 2)
	{
		printf("Usage: sim foo.elf\n");
		return 0;
	}

	//Read and map the file
	struct virtual_mem_region* memory = NULL;
	struct context ctx;
	ReadELF(argv[1], &memory, &ctx);

	//Run the CPU
	RunSimulator(memory, &ctx);

	//TODO: clean up

	return 0;
}
