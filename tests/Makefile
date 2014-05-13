FILES=addi addiu andi
all:
	for f in $(FILES) ; do \
		mipsel-linux-gnu-gcc $$f.S -o $$f.elf -nostdlib -nostartfiles -fno-delayed-branch ; \
	done
	gcc test.c ../sim/sim.c -o test --std=c99
