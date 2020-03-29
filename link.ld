ENTRY(bootstrap)
OUTPUT_FORMAT("binary")
SECTIONS {
    . = 0xf000;

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
