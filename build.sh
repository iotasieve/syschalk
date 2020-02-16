nasm -f elf32 boot1.asm -o boot1.o
clang --target=i686-pc-none-elf -march=i686 kmain.c boot1.o -o kernel.bin -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -Werror -T link.ld
qemu-system-x86_64 -fda kernel.bin

