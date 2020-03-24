section .boot
bits 16
global boot
boot:
    ; enable a20
    mov ax, 0x2401
    int 0x15
    ; set vga text mode 
    mov ax, 0x3
    int 0x10
    
    mov [disk], dl

    ; read other sectors from the disk (.bin file)
    mov ah, 0x2 ; read sectors operation
    mov al, 60 ; how much
    mov ch, 0 ; cylinder
    mov dh, 0 ; head
    mov cl, 2 ; sector
    mov dl, [disk] ; disk
    mov bx, extern_code ; where on RAM copy it
    int 0x13 ; the interrupt itself

    ; load GDT
    cli
    lgdt [gdt_pointer]
    
    ; use protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
   
    ; jump into the code that jumps into the kernel 
    jmp CODE_SEG:kernel_jump

disk:
    db 0
gdt_start:
    dq 0x0 ; null entry
gdt_code:
    dw 0xFFFF ; limit (low byte)
    dw 0 ; base low
    db 0 ; base mid
    db 10011010b ; look GDT access layout
    db 11001111b ; look GDT flags db 0 ; base end
    db 0
gdt_data:
    dw 0xFFFF ; limit (low byte)
    dw 0 ; base low
    db 0 ; base mid
    db 10010010b ; look GDT access layout
    db 11001111b ; look GDT flags
    db 0 ; base end
gdt_end:

gdt_pointer:
    dw gdt_end - gdt_start
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510 - ($-$$) db 0
dw 0xaa55

bits 32
extern_code:
kernel_jump:
    ; init data segments
    mov ax, DATA_SEG,
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    extern kmain
    call kmain
    cli
    hlt
section .bss
align 4
kernel_stack_bottom: equ $
    resb 16384; reserve 16 kb of stack
kernel_stack_top:
