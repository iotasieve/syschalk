#include <inc/irq_handlers.h>

bool ataLock = false;

s2_UInt32 s2_msUptime = 0;
s2_UInt32 s2_sysDelta = 0;

void irq0_handler(void) 
{
    // s2_OutB(0x43, 0);
    s2_msUptime += 1;
    if (s2_IOPITCountdown > 0)
        s2_IOPITCountdown -= 1;
    s2_OutB(0x20, 0x20); //EOI
}


void irq1_handler(void) 
{ 
    s2_PS2KeyboardHandler();
    s2_OutB(0x20, 0x20); //EOI
}

void irq2_handler(void) 
{
    s2_OutB(0x20, 0x20); //EOI
}

void irq3_handler(void) 
{
    s2_OutB(0x20, 0x20); //EOI
}

void irq4_handler(void) 
{
    s2_OutB(0x20, 0x20); //EOI
}

void irq5_handler(void) 
{
    s2_OutB(0x20, 0x20); //EOI
}

void irq6_handler(void) 
{
    s2_OutB(0x20, 0x20); //EOI
}

void irq7_handler(void) 
{
    s2_OutB(0x20, 0x20); //EOI
}

void irq8_handler(void) 
{
    s2_OutB(0xA0, 0x20);
    s2_OutB(0x20, 0x20); //EOI          
}

void irq9_handler(void) 
{
    s2_OutB(0xA0, 0x20);
    s2_OutB(0x20, 0x20); //EOI
}

void irq10_handler(void) 
{
    s2_OutB(0xA0, 0x20);
    s2_OutB(0x20, 0x20); //EOI
}

void irq11_handler(void) 
{
    s2_OutB(0xA0, 0x20);
    s2_OutB(0x20, 0x20); //EOI
}

void irq12_handler(void) 
{
    s2_OutB(0xA0, 0x20); 
    s2_OutB(0x20, 0x20); //EOI 
}

void irq13_handler(void) 
{
    s2_OutB(0xA0, 0x20);
    s2_OutB(0x20, 0x20); //EOI
}

void irq14_handler(void) 
{
    s2_InB(0x1F0+0x7);
    ataLock = true;
    s2_OutB(0xA0, 0x20);
    s2_OutB(0x20, 0x20); //EOI
}

void irq15_handler(void) 
{
    asm("cli");
    s2_InB(0x170+0x7);
    ataLock = true;

    asm("sti");
    s2_OutB(0xA0, 0x20);
    s2_OutB(0x20, 0x20); //EOI
}

