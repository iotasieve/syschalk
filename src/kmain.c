
#include <inc/mem/manager.h>
#include <inc/drivers/ps2_keyboard.h>
#include <inc/events/events.h>
#include <inc/idt.h>
#include <inc/io.h>
#include <inc/irq_handlers.h>
#include <inc/port.h>
#include <inc/pci/pci.h>
#include <inc/programs/shell/shell.h>
#include <inc/drivers/vga/vga.h>
#define PIC_MASTER 0x20
#define PIC_MASTER_DATA (PIC_MASTER + 1)
#define PIC_SLAVE 0xA0
#define PIC_SLAVE_DATA (PIC_SLAVE + 1)

// TODO: CHANGE THIS void cleanscr(char color)


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
    for (int i = 0; i < millis * 100000; ++i) {
        asm volatile("nop");
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

// void masks_hud_draw(char *r0, char *r1)
// {
//     to_bin(r0, s2_InB(PIC_MASTER_DATA));
//     to_bin(r1, s2_InB(PIC_SLAVE_DATA));
//     s2_TVMPrint("MASTER ", 0x21, 2000-80-8-7);
//     s2_TVMPrint(r0, 0x21, 2000-80-8);
    
//     s2_TVMPrint("SLAVE  ", 0x21, 2000-8-7);
//     s2_TVMPrint(r1, 0x21, 2000-8);
// }

void PIC_init()
{
    char master_mask = s2_InB(PIC_MASTER_DATA);
    char slave_mask = s2_InB(PIC_SLAVE_DATA);

    s2_OutB(PIC_MASTER, 0x11); // initialization sequence start
    sleep(1);
    s2_OutB(PIC_SLAVE, 0x11);
    sleep(1);
    s2_OutB(PIC_MASTER_DATA, 32); // set offset
    sleep(1);
    s2_OutB(PIC_SLAVE_DATA, 40);
    sleep(1);
    s2_OutB(PIC_MASTER_DATA, 4); // tell master PIC about slave PIC at IRQ 2
    sleep(1);
    s2_OutB(PIC_SLAVE_DATA, 2); // tell slave PIC its cascade identity (whatever that supposed to mean)
    sleep(1);
    // Additional parameters ( I suppose tells mode of PIC, in this case 8086/88 (MCS-80/85) mode ) 
    s2_OutB(PIC_MASTER_DATA, 1);
    sleep(1);
    s2_OutB(PIC_SLAVE_DATA, 1);
    sleep(1);
    s2_OutB(PIC_MASTER_DATA, master_mask);
    s2_OutB(PIC_SLAVE_DATA, slave_mask);
}



void s2_MemoryTest()
{
    // Root entry test
    char *a = s2_MemoryAlloc(200);
    if ((s2_MemoryEntry*)(a-sizeof(s2_MemoryEntry)) == rootEntry)
    {
        s2_TVMPrint("Memory Test | Root Entry | PASSED", 0x70, 0);
    }
    else 
    {
        s2_TVMPrint("Memory Test | Root Entry | FAILED", 0x70, 0);
        
        s2_TVMPrint(s2_ToHex((unsigned int)(a-sizeof(s2_MemoryEntry))), 0x70, 240);
        s2_TVMPrint(s2_ToHex((unsigned int)rootEntry), 0x70, 240);
    }

    // Same-Entry test
    char *b = s2_MemoryAlloc(100);
    s2_MemoryFree(b);
    char *c = s2_MemoryAlloc(50);
    if (b == c) 
    {
        s2_TVMPrint("Memory Test | Pre-Made Node | PASSED", 0x70, 80);
    }
    else 
    {    
        s2_TVMPrint("Memory Test | Pre-Made Node | FAILED", 0x70, 80);
        s2_TVMPrint(s2_ToHex((unsigned int)b), 0x70, 240);
        s2_TVMPrint(s2_ToHex((unsigned int)c), 0x70, 240);
    }

    // New entry test
    char *d = s2_MemoryAlloc(100);

    if ((d-100) != b)
    {
        s2_TVMPrint("Memory Test | New-Node | PASSED", 0x70, 160);
    }
    else 
    {
        s2_TVMPrint("Memory Test | New-Node | FAILED", 0x70, 160);
        s2_TVMPrint(s2_ToHex((unsigned int)d-100), 0x70, 240);
        s2_TVMPrint(s2_ToHex((unsigned int)b), 0x70, 320);
    }

    s2_MemoryFree(d);
    s2_MemoryFree(c);

    char *e = s2_MemoryAlloc(150);
    if (e == b)
    {
        s2_TVMPrint("Memory Test | Merge-Node | PASSED", 0x70, 240);
    }
    else
    {
        s2_TVMPrint("Memory Test | Merge-Node | FAILED", 0x70, 240);
        s2_TVMPrint(s2_ToHex((unsigned int)e), 0x70, 320);
        s2_TVMPrint(s2_ToHex((unsigned int)b), 0x70, 400);
  
    }

    // Free test
    s2_MemoryFree(e);
    if (((s2_MemoryEntry*)(e-sizeof(s2_MemoryEntry)))->flags & S2_MEMFLAG_ISALLOC)
    {
        s2_TVMPrint("Memory Test | Free memory | FAILED", 0x70, 800);
    }
}

extern void kmain()
{
    PIC_init();
    IDT_init();
    s2_InitMemoryAllocator();
    s2_EventQueueInit();

    s2_PCIDevicesScanBruteforce();
    if (!s2_supportsVGA)
    {
        s2_Panic(S2_PANICERR_NOVGASUPPORT, "Your PC/Emulator Does not support VGA", true, __FILE__, s2_ToHex(__LINE__));
    }
    s2_TVMPrint("VGA compatible card detected, switching modes", 0x70, 0);
    
    s2_Byte *scrBuffer = s2_MemoryAlloc(320*200);
    

    s2_VGASetMode(320, 200, 8);
    s2_VGAReprogramPallete8();
    
    for (int i = 0; i < 320*200; i++)
    {
        scrBuffer[i] = i/256;
    }

    s2_VGADrawBuffer8I(scrBuffer);
}
