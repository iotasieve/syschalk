#include <inc/drivers/ps2_keyboard.h>
#include "events/events.c"
#include <inc/idt.h>
#include <inc/io.h>
#include <inc/irq_handlers.h>
#include <inc/pic.h>
#include "mem/manager.c"

#define PIC_MASTER 0x20
#define PIC_MASTER_DATA (PIC_MASTER + 1)
#define PIC_SLAVE 0xA0
#define PIC_SLAVE_DATA (PIC_SLAVE + 1)

// TODO: CHANGE THIS void cleanscr(char color)
void cleanscr(char color)
{
    s2_TVMPrint("                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ", color, 0);
}

void draw_circle(int r, char color)
{
    for (int i = 0; i < (2*r); i++)
    {
        for (int j = 0; j < (2*r); j++)
        {
            int d = (i*i)+(j*j); if ((d+90) < r*r)
            {   
                s2_TVMPrint("#", color, i+(80*j));
            }
            else if (d < r*r)
            {
                s2_TVMPrint("#", ((color & 0x0F) | 0x3F), i+(80*j));
            }
        }
    }
}
void sleep(short millis)
{
    for (int i = 0; i < millis * 10000000; ++i) {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }
}

char* to_bin(char *res, unsigned char byte)
{
    res[0] = '0'+((byte & (char)0b10000000) >> 7);
    res[1] = '0'+((byte & (char)0b01000000) >> 6);
    res[2] = '0'+((byte & (char)0b00100000) >> 5);
    res[3] = '0'+((byte & (char)0b00010000) >> 4);
    res[4] = '0'+((byte & (char)0b00001000) >> 3);
    res[5] = '0'+((byte & (char)0b00000100) >> 2);
    res[6] = '0'+((byte & (char)0b00000010) >> 1);
    res[7] = '0'+(byte & (char)0b00000001);
    res[8] = '\0';
    return res;
}

void masks_hud_draw(char *r0, char *r1)
{
    to_bin(r0, s2_In8(PIC_MASTER_DATA));
    to_bin(r1, s2_In8(PIC_SLAVE_DATA));
    s2_TVMPrint("MASTER ", 0x21, 2000-80-8-7);
    s2_TVMPrint(r0, 0x21, 2000-80-8);
    
    s2_TVMPrint("SLAVE  ", 0x21, 2000-8-7);
    s2_TVMPrint(r1, 0x21, 2000-8);
}

void PIC_init()
{
    char master_mask = s2_In8(PIC_MASTER_DATA);
    char slave_mask = s2_In8(PIC_SLAVE_DATA);

    s2_Out8(PIC_MASTER, 0x11); // initialization sequence start
    sleep(1);
    s2_Out8(PIC_SLAVE, 0x11);
    sleep(1);
    s2_Out8(PIC_MASTER_DATA, 32); // set offset
    sleep(1);
    s2_Out8(PIC_SLAVE_DATA, 40);
    sleep(1);
    s2_Out8(PIC_MASTER_DATA, 4); // tell master PIC about slave PIC at IRQ 2
    sleep(1);
    s2_Out8(PIC_SLAVE_DATA, 2); // tell slave PIC its cascade identity (whatever that supposed to mean)
    sleep(1);
    // Additional parameters ( I suppose tells mode of PIC, in this case 8086/88 (MCS-80/85) mode ) 
    s2_Out8(PIC_MASTER_DATA, 1);
    sleep(1);
    s2_Out8(PIC_SLAVE_DATA, 1);
    sleep(1);
    s2_Out8(PIC_MASTER_DATA, master_mask);
    s2_Out8(PIC_SLAVE_DATA, slave_mask);
}

char* s2_ToHex(unsigned int num)
{
    char *res = (char*)s2_MemoryAlloc(sizeof(char)*9);
    int i = 0;
    do {
        unsigned char rem = num % 16;
        num /= 16;
        res[i] = "0123456789ABCDEF"[rem];
        i += 1;
    } while (num != 0);
    res[8] = 0;
    return res;
}

extern void kmain()
{
    PIC_init();
    IDT_init();
    s2_InitMemoryAllocator();
    char *a = (char*)s2_MemoryAlloc(sizeof(char)*200);
    char *b = (char*)s2_MemoryAlloc(sizeof(char)*54);
    char *c = (char*)s2_MemoryAlloc(sizeof(char)*20);
    char *d = (char*)s2_MemoryAlloc(sizeof(char)*20);
    s2_MemoryFree(c);
    s2_MemoryPurge(d);
    char *e = (char*)s2_MemoryAlloc(sizeof(char)*10);
    char *f = (char*)s2_MemoryAlloc(sizeof(char)*50);
    s2_MemoryFree(f);
    char *g = (char*)s2_MemoryAlloc(sizeof(char)*45);
    g = "This string used freed mem";
    e = "This";
    a = "This string uses root node";
    b = "This string uses newly created node";
    f[4] = 'L';
    cleanscr(0x11);
   
    s2_TVMPrint(s2_ToHex((unsigned int)f), 0x04, 510);
    
    extern unsigned int heap_start;
    s2_TVMPrint(s2_ToHex((unsigned int)g), 0x04, 500);


    while (1)
    {
        s2_TVMPrint(g, 0x03, 0);
        s2_TVMPrint(e, 0x03, 80);
        s2_TVMPrint(a, 0x03, 160);
        s2_TVMPrint(b, 0x03, 240);
    }
}
