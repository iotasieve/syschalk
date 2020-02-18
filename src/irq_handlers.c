#include <inc/pic.h>
#include <inc/io.h>

char keyp[2];

void irq0_handler(void) {
    out8(0x20, 0x20); //EOI
}

// Just for testing
void irq1_handler(void) {
    keyp[0] = in8(0x60);
    keyp[1] = '\0';
    printvmem(keyp, 0x34, 0);
    out8(0x20, 0x20); //EOI
}

void irq2_handler(void) {
    out8(0x20, 0x20); //EOI
}

void irq3_handler(void) {
    out8(0x20, 0x20); //EOI
}

void irq4_handler(void) {
    out8(0x20, 0x20); //EOI
}

void irq5_handler(void) {
    out8(0x20, 0x20); //EOI
}

void irq6_handler(void) {
    out8(0x20, 0x20); //EOI
}

void irq7_handler(void) {
    out8(0x20, 0x20); //EOI
}

void irq8_handler(void) {
    out8(0xA0, 0x20);
    out8(0x20, 0x20); //EOI          
}

void irq9_handler(void) {
    out8(0xA0, 0x20);
    out8(0x20, 0x20); //EOI
}

void irq10_handler(void) {
    out8(0xA0, 0x20);
    out8(0x20, 0x20); //EOI
}

void irq11_handler(void) {
    out8(0xA0, 0x20);
    out8(0x20, 0x20); //EOI
}

void irq12_handler(void) {
    out8(0xA0, 0x20); 
    out8(0x20, 0x20); //EOI 
}

void irq13_handler(void) {
    out8(0xA0, 0x20);
    out8(0x20, 0x20); //EOI
}

void irq14_handler(void) {
    out8(0xA0, 0x20);
    out8(0x20, 0x20); //EOI
}

void irq15_handler(void) {
    out8(0xA0, 0x20);
    out8(0x20, 0x20); //EOI
}

