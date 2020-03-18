#include <inc/pic.h>
#include <inc/io.h>
#include "drivers/ps2_keyboard.c"

char keyp[2];

void irq0_handler(void) 
{
    s2_Out8(0x20, 0x20); //EOI
}

void irq1_handler(void) 
{ 
    s2_PS2KeyboardHandler();
    s2_Out8(0x20, 0x20); //EOI
}

void irq2_handler(void) 
{
    s2_Out8(0x20, 0x20); //EOI
}

void irq3_handler(void) 
{
    s2_Out8(0x20, 0x20); //EOI
}

void irq4_handler(void) 
{
    s2_Out8(0x20, 0x20); //EOI
}

void irq5_handler(void) 
{
    s2_Out8(0x20, 0x20); //EOI
}

void irq6_handler(void) 
{
    s2_Out8(0x20, 0x20); //EOI
}

void irq7_handler(void) 
{
    s2_Out8(0x20, 0x20); //EOI
}

void irq8_handler(void) 
{
    s2_Out8(0xA0, 0x20);
    s2_Out8(0x20, 0x20); //EOI          
}

void irq9_handler(void) 
{
    s2_Out8(0xA0, 0x20);
    s2_Out8(0x20, 0x20); //EOI
}

void irq10_handler(void) 
{
    s2_Out8(0xA0, 0x20);
    s2_Out8(0x20, 0x20); //EOI
}

void irq11_handler(void) 
{
    s2_Out8(0xA0, 0x20);
    s2_Out8(0x20, 0x20); //EOI
}

void irq12_handler(void) 
{
    s2_Out8(0xA0, 0x20); 
    s2_Out8(0x20, 0x20); //EOI 
}

void irq13_handler(void) 
{
    s2_Out8(0xA0, 0x20);
    s2_Out8(0x20, 0x20); //EOI
}

void irq14_handler(void) 
{
    s2_Out8(0xA0, 0x20);
    s2_Out8(0x20, 0x20); //EOI
}

void irq15_handler(void) 
{
    s2_Out8(0xA0, 0x20);
    s2_Out8(0x20, 0x20); //EOI
}

