FILES=addi addiu andi ori xori lui slti sltiu
.PHONY: all c assembly
all:
	gcc test.c ../sim/sim.c -o test --std=c99
	for f in $(FILES) ; do \
		mipsel-linux-gnu-gcc $$f.S -o $$f.elf -nostdlib -nostartfiles -fno-delayed-branch ; \
	done
c:
	gcc test.c ../sim/sim.c -o test --std=c99
assembly:
	for f in $(FILES) ; do \
		mipsel-linux-gnu-gcc $$f.S -o $$f.elf -nostdlib -nostartfiles -fno-delayed-branch ; \
	done
