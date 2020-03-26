all:
	nasm -f elf32 ./src/boot1.asm -o ./bin/boot1.o
	nasm -f elf32 ./src/irq.asm -o ./bin/irq.o
	clang -O2 -I. --target=i686-pc-none-elf -march=i686 -c ./src/**/**/**.c ./src/**.c ./src/**/**.c -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nostdlib 
	-fno-rtti -Wall -Wextra -Werror 
	mv ./*.o ./bin/obj
	ld -m elf_i386 ./bin/boot1.o ./bin/irq.o ./bin/obj/* -o ./bin/kernel.bin -T link.ld
	qemu-system-x86_64 -drive if=floppy,readonly,index=0,format=raw,file=./bin/kernel.bin -vga qxl -net nic,vlan=0,macaddr=e2:ed:a2:e6:a2:13
	

