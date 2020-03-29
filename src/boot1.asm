bits 16
org 0x7c00
Boot:
    ; enable a20
    mov ax, 0x2401
    int 0x15
    ; set vga text mode 
    mov ax, 0x3
    int 0x10
    jmp LoadISO9660Kernel	

    
    
   
       ; jump into the code that jumps into the kernel 
ReadConfig:
    db 16 ; size of packet
    db 0
    dw 16 ; sectors
RAMDestination: dd 0
LBAConfig:    
    dw 16 ; low
    dw 0 ; hi
    dd 0 ;hi

LoadISO9660Kernel:
    mov ax, [KernelUnloadLocation]
    mov [RAMDestination], ax

    ; read other sectors from the .iso file
    mov si, ReadConfig
    mov ah, 0x42 ; read sectors operation
    mov dl, [DiskNumber] ; disk
    int 0x13  ; bios int
    

    mov bx, [KernelUnloadLocation] ; load ISO
    add bx, 158 ; get LBA location
    mov ax, [bx] ; new LBA offset

    mov [LBAConfig], ax ; move LBA offset
    mov si, ReadConfig
    mov ah, 0x42 ; read sectors operation
    mov dl, [DiskNumber] ; disk
    int 0x13  ; bios int
 
    mov bx, [KernelUnloadLocation]
    ; Poll for first character of every file
CheckFilename:
    ; Directory table, 33th byte indicates filename
    
    mov cx, bx
    add cx, 33
    mov di, cx

    mov al, [di] ; Check if first character is K, TODO: (See trello)
    cmp al, 'K'
    je CheckEnd ; found filename starting with that character

    ; otherwise we add size which is specified in first byte of directory entry
    xor ax, ax
    mov al, [bx]
    add bx, ax
    jmp CheckFilename

CheckEnd:
    add bx, 2
    mov dx, [bx]
    mov [LBAConfig], dx ; move LBA offset
    mov si, ReadConfig
    mov ah, 0x42 ; read sectors operation
    mov dl, [DiskNumber] ; disk
    int 0x13  ; bios int
 


  ; load GDT
    cli
    lgdt [GDTPointer]


    ; At this point we can just switch to protected mode
    ; use protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
   

   
    jmp CODE_SEG:kernel_jump

DiskNumber:
    db 0xe0
GDTStart:
    dq 0x0 ; null entry
GDTCode:
    dw 0xFFFF ; limit (low byte)
    dw 0 ; base low
    db 0 ; base mid
    db 10011010b ; look GDT access layout
    db 11001111b ; look GDT flags db 0 ; base end
    db 0
GDTData:
    dw 0xFFFF ; limit (low byte)
    dw 0 ; base low
    db 0 ; base mid
    db 10010010b ; look GDT access layout
    db 11001111b ; look GDT flags
    db 0 ; base end
GDTEnd:

GDTPointer:
    dw GDTEnd - GDTStart
    dd GDTStart

CODE_SEG equ GDTCode - GDTStart
DATA_SEG equ GDTData - GDTStart

bits 32
kernel_jump:
    ; init data segments
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax    
    mov gs, ax
    mov ss, ax

    ; TODO: Jump into kernel.bin after finding
    
    call [KernelUnloadLocation]; Jump to kernel

    cli
    hlt
times 510 - ($-$$) db 0
dw 0xaa55

KernelUnloadLocation:
    dd 0xf000














































































