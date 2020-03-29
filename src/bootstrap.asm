section .bootstrap
global bootstrap
bootstrap:
    extern kmain
    call kmain
    ret
db 'BOOTSTRAPMAGIC'
section .bss
align 4
kernel_stack_bottom: equ $
    resb 32768; reserve 32 kb of stack
kernel_stack_top:
