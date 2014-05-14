FILES=addi addiu andi ori xori lui slti sltiu sw lw sb lb beq bne
.PHONY: all c assembly
all:
	gcc *.c ../sim/sim.c -o test --std=c99
	for f in $(FILES) ; do \
		mipsel-linux-gnu-gcc MIPS_tests/$$f.S -o MIPS_tests/$$f.elf -nostdlib -nostartfiles -fno-delayed-branch ; \
	done
c:
	gcc *.c ../sim/sim.c -o test --std=c99
assembly:
	for f in $(FILES) ; do \
		mipsel-linux-gnu-gcc MIPS_tests/$$f.S -o MIPS_tests/$$f.elf -nostdlib -nostartfiles -fno-delayed-branch ; \
	done
