#include <inc/idt.h>

#include <inc/io.h>
#include <inc/irq_handlers.h>
#include <inc/pic.h>

#define PIC_MASTER 0x20
#define PIC_MASTER_DATA (PIC_MASTER + 1)
#define PIC_SLAVE 0xA0
#define PIC_SLAVE_DATA (PIC_SLAVE + 1)

// TODO: CHANGE THIS void cleanscr(char color)
void cleanscr(char color)
{
    printvmem("                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ", color, 0);
}

void draw_circle(int r, char color)
{
    for (int i = 0; i < (2*r); i++)
    {
        for (int j = 0; j < (2*r); j++)
        {
            int d = (i*i)+(j*j); if ((d+90) < r*r)
            {   
                printvmem("#", color, i+(80*j));
            }
            else if (d < r*r)
            {
                printvmem("#", ((color & 0x0F) | 0x3F), i+(80*j));
            }
        }
    }
}
void sleep(short millis)
{
    for (int i = 0; i < millis * 100000; ++i) {}
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
    to_bin(r0, in8(PIC_MASTER_DATA));
    to_bin(r1, in8(PIC_SLAVE_DATA));
    printvmem("MASTER ", 0x21, 2000-80-8-7);
    printvmem(r0, 0x21, 2000-80-8);
    
    printvmem("SLAVE  ", 0x21, 2000-8-7);
    printvmem(r1, 0x21, 2000-8);
}

void PIC_init()
{
    char master_mask = in8(PIC_MASTER_DATA);
    char slave_mask = in8(PIC_SLAVE_DATA);

    out8(PIC_MASTER, 0x11); // initialization sequence start
    sleep(1);
    out8(PIC_SLAVE, 0x11);
    sleep(1);
    out8(PIC_MASTER_DATA, 32); // set offset
    sleep(1);
    out8(PIC_SLAVE_DATA, 40);
    sleep(1);
    out8(PIC_MASTER_DATA, 4); // tell master PIC about slave PIC at IRQ 2
    sleep(1);
    out8(PIC_SLAVE_DATA, 2); // tell slave PIC its cascade identity (whatever that supposed to mean)
    sleep(1);
    // Additional parameters ( I suppose tells mode of PIC, in this case 8086/88 (MCS-80/85) mode ) 
    out8(PIC_MASTER_DATA, 1);
    sleep(1);
    out8(PIC_SLAVE_DATA, 1);
    sleep(1);
    out8(PIC_MASTER_DATA, master_mask);
    out8(PIC_SLAVE_DATA, slave_mask);
}

extern void kmain()
{
    PIC_init();
    idt_init();
    char r0[9], r1[9];
    
    while (1)
    {
        masks_hud_draw(r0, r1);   
    }
}
