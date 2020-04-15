all:
	make boot
	make kernel
	make iso 
	make run
boot:
	nasm -f bin ./src/boot1.asm -o ./bin/boot.bin

kernel:
	nasm -f elf32 ./src/irq.asm -o ./bin/irq.o
	nasm -f elf32 ./src/bootstrap.asm -o ./bin/bootstrap.o
	clang -Oz -I. -std=gnu11 --target=i686-pc-none-elf -march=i686 -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nodefaultlibs -fno-builtin -fno-builtin-memcpy -c ./src/**/**/**.c ./src/**.c ./src/**/**.c 
	-fno-rtti -Wall -Wextra -Werror 
	mkdir -p ./bin/obj
	mv ./*.o ./bin/obj
	ld -m elf_i386 ./bin/bootstrap.o ./bin/irq.o ./bin/obj/* -o ./bin/kernel.bin -T link.ld

iso:
	cp ./bin/kernel.bin ./bin/cd
	dd if=./bin/boot.bin of=./bin/cd/boot.img bs=512 count=2880
	mkisofs -no-emul-boot -boot-load-size 4 -o ./bin/s2cd.iso -V "s2 Commit Number 15" -b boot.img ./bin/cd 

run:
	qemu-system-i386 -machine pc -debugcon stdio -cdrom ./bin/s2cd.iso -m 512M -vga qxl
	# qemu-system-x86_64 -drive if=floppy,readonly,index=0,format=raw,file=./bin/kernel.bin -vga vmware -net nic,vlan=0,macaddr=e2:ed:a2:e6:a2:13

