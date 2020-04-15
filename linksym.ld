ENTRY(bootstrap)
OUTPUT_FORMAT("elf32-i386")
SECTIONS {
    . = 0xF000;

    .text :
    {
        *(.bootstrap)
        *(.text)
    }
    .rodata : 
    {
        *(.rodata)
    }
    .data : 
    {
        *(.data)
    }
    .bss : 
    {
        *(.bss)    
    }
}
