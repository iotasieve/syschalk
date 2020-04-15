
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
#include <inc/fs/iso9660/iso9660.h>
#include <inc/log.h>
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
    for (int i = 0; i < millis * 10; ++i) {
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
    s2_IOWait();
    s2_OutB(PIC_SLAVE, 0x11);
    s2_IOWait();
    s2_OutB(PIC_MASTER_DATA, 32); // set offset
    s2_IOWait();
    s2_OutB(PIC_SLAVE_DATA, 40);
    s2_IOWait();
    s2_OutB(PIC_MASTER_DATA, 4); // tell master PIC about slave PIC at IRQ 2
    s2_IOWait();
    s2_OutB(PIC_SLAVE_DATA, 2); // tell slave PIC its cascade identity (whatever that supposed to mean)
    s2_IOWait();
    // Additional parameters ( I suppose tells mode of PIC, in this case 8086/88 (MCS-80/85) mode ) 
    s2_OutB(PIC_MASTER_DATA, 1);
    s2_IOWait();
    s2_OutB(PIC_SLAVE_DATA, 1);
    s2_IOWait();
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
void IRQ_clear_mask(unsigned char IRQline) {
    s2_UInt16 port;
    s2_UInt16 value;
 
    if(IRQline < 8) {
        port = PIC_MASTER_DATA;
    } else {
        port = PIC_SLAVE_DATA;
        IRQline -= 8;
    }
    value = s2_InB(port) & ~(1 << IRQline);
    s2_OutB(port, value);        
}

#define S2_PAGE_PRESENT_BIT 1

void s2_CreatePageDirectory(s2_Size *pdeloc)
{
    s2_Log("PageDirectory", "Creating page table", S2_LOGCOLOR_INFO);

    for (int i = 0; i < 1024; i++)
        pdeloc[i] = 0;
}


void s2_CreatePageTable(s2_Size *tblloc)
{
    s2_Log("PageTable", "Creating page table", S2_LOGCOLOR_INFO);

    for (int i = 0; i < 1024; i++)
        tblloc[i] = 0;
}

void s2_PageMap(s2_Size *pde, s2_Size vaddr, s2_Size paddr)
{
    s2_Size *ptr;

    if (!(pde[(vaddr >> 22)] & S2_PAGE_PRESENT_BIT))
    {
        s2_Log("PageMap", "Page not present", S2_LOGCOLOR_INFO);
        s2_CreatePageTable(pde[1024 * (vaddr >> 22)]);
    }
}

extern void kmain()
{
    // s2_InitMemoryAllocator();
    // PIC_init();
    // s2_OutB(PIC_MASTER_DATA, 0);
    // s2_OutB(PIC_SLAVE_DATA, 0);
    // IDT_init();

    s2_LogTest();

    int esp;
    extern unsigned int kernel_stack_bottom;
    asm("movl %%esp, %0" : "=r"(esp));
    s2_QEMUDebugPrint(s2_ToDecNoA(&kernel_stack_bottom));
    s2_QEMUDebugPrint(s2_ToDecNoA(esp));

    // s2_CreatePageDirectory(heap_start);


    // s2_EventQueueInit();
    // s2_PITTimerInit();
    // s2_PCIDevicesScanBruteforce();

    // // s2_PCIDevicesScanBruteforce();
    // // Scan of IDE controller
    // s2_PCIDeviceDescriptor *a = s2_PCIScanFor(1, 1, -1);
    
    // if (a != NULL)
    // {
    //     s2_TVMPrintA("Found IDE-Compatible device");
    //     if ((a->interruptLine & 0xFF) == 0xFE)
    //     {
    //         s2_TVMPrintA("This device requires IRQ assignment (unhandled)");
    //     }
    //     else
    //     {
    //         s2_TVMPrintA("This device doesn't require IRQ assignment");
    //         if (a->classCode == 0x01 && a->subclass == 0x01 && (a->progIF == 0x8A || a->progIF == 0x80))
    //         {
    //             s2_TVMPrintA("This is parallel IDE controller which uses IRQ 14 and IRQ 15");
    //             s2_IDEDevice *dev;
    //             bool found = false;
    //             for (int i = 0; i < 2; i++)
    //             {
    //                 for (int j = 0; j < 2; j++)
    //                 {
    //                     dev = s2_IDECreateATADeviceInstance(a, i, j);
    //                     if (dev->exists)
    //                     {
    //                         if (dev->type == S2_ATA_TYPE_ATAPI)
    //                         {
    //                             found = true;
    //                             goto enddev;
    //                         }
    //                     }
    //                 }
    //             }

    //             enddev:

    //             if (found)
    //             {
    //                 s2_TVMPrintA("Found ATAPI device");
    //                 s2_ISO9660FS *fs = s2_ISO9660InstCreate(dev);
    //                 gFS = fs;
                     
                
    //                 if (!s2_supportsVGA)
    //                 {
    //                     s2_Panic(S2_PANICERR_NOVGASUPPORT, "Your PC/Emulator Does not support VGA", true, __FILE__, s2_ToHex(__LINE__));
    //                 }
    //                 s2_TVMPrint("VGA compatible card detected, switching modes", 0x70, 0);
                    
    //                 // s2_Byte *scrBuffer = s2_MemoryAlloc(320*200);
                    

    //                 s2_VGASetMode(320, 200, 8);
    //                 s2_VGAReprogramPallete8();
                    
    //                 GMain();
    //             }
    //             else
    //             {
    //                 s2_TVMPrintA("Couldn't find CD-ROM System halted");
    //             }
    //         }
    //     }
        
    // }
    // else
    // {
    //     s2_TVMPrintA("No IDE-Compatible device found, system halted");
    // }
    
    

    

    // s2_VGADrawBuffer8I(scrBuffer);
    
}
