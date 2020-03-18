all:
	nasm -f elf32 ./src/boot1.asm -o ./bin/boot1.o
	nasm -f elf32 ./src/irq.asm -o ./bin/irq.o
	clang -O2 -I. --target=i686-pc-none-elf -march=i686 ./src/*.c  ./bin/boot1.o ./bin/irq.o -o ./bin/kernel.bin -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror -T link.ld
	qemu-system-x86_64 -fda ./bin/kernel.bin

