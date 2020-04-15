#include <inc/io.h>
#include <inc/port.h>

// Forward delcare
void* s2_MemoryAlloc(s2_Size size);
void* s2_MemoryFree(void *ptr);

int linen = 0;
bool lolmode = false;
short *VGA_PTR = (short *)0xb8000;

s2_UInt32 s2_IOPITCountdown = 0;

void s2_TVMPrint(char *str, char color, short offset)
{
    int i = 0;
    while (str[i] != 0)
    {
        if (!lolmode)
            VGA_PTR[i + offset] = (color << 8) | str[i];
        else
            VGA_PTR[i + offset] = ((((0x01+(i+offset))&0x0F)+1) << 8) | str[i];
        ++i;
    }
}

void s2_Panic(unsigned short code, char* message, bool isFatal, char* file, char* line)
{
    s2_TVMClear(0x00);
    s2_TVMPrint("OUCH! System panic due to: ", 0x04, 0);
    s2_TVMPrint(message, 0x04, 80);
    s2_TVMPrint("in file ", 0x04, 160);
    s2_TVMPrint(file, 0x04, 168);
    s2_TVMPrint("at line ", 0x04, 240);
    s2_TVMPrint(line, 0x04, 248);
    
    switch (code)
    {
    case S2_PANICERR_GENERATED:
        s2_TVMPrint("S2_PANICERR_GENERATED", 0x04, 480);
        break;
    case S2_PANICERR_MEMALLOC:
        s2_TVMPrint("S2_PANICERR_MEMALLOC", 0x04, 480);
        break;
    case S2_PANICERR_NOVGASUPPORT:
        s2_TVMPrint("S2_PANICERR_NOVGASUPPORT", 0x04, 480);
        break;
    default:
        s2_TVMPrint("S2_UNKNOWN_ERROR", 0x04, 480);
        break;
    }
    
    if (isFatal)
    {
        s2_TVMPrint("The system will NOT continue working (FATAL)", 0x04, 400);
        asm("cli");
        asm("hlt");
    }
    else
    {
        s2_TVMPrint("The system will continue working", 0x04, 400);
    }
}

char* s2_ToHex(unsigned int num)
{
    char *res = (char*)s2_MemoryAlloc(sizeof(char)*9);
    int i = 0;
    do {
        unsigned char rem = num % 16;
        num /= 16;
        res[7-i] = "0123456789ABCDEF"[rem];
        i += 1;
    } while (i < 8);
    res[8] = 0;
    return res;
}

char* s2_ToDec(unsigned int num)
{
    char *res = (char*)s2_MemoryAlloc(sizeof(char)*17);
    int i = 0;
    int scnt = 0;
    do {
        unsigned char rem = num % 10;
        num /= 10;
        if (num == 0 && rem == 0 && i != 0)
        {
            res[15-i] = ' ';
            i += 1;
            scnt += 1;
            continue;
        }
        res[15-i] = "0123456789"[rem];
        i += 1;
    } while (i < 16);
    for (int i = 0; i < 16; i++)
    {
        res[i] = res[i+scnt];
        res[i+1] = 0;
    }
    return res;
}

void s2_TVMPrintA(char *str)
{
    s2_TVMPrint(str, 0x07, linen*80);
    linen += 1;
}

void s2_TVMClear(char color)
{
    s2_TVMPrint("                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ", color, 0);
}

void s2_PITSleep(s2_UInt32 ms)
{
    s2_IOPITCountdown = ms;
    while(s2_IOPITCountdown > 0)
    {
        asm("hlt");
    }
}

inline void s2_IOWait(void)
{
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}

void s2_PITTimerInit()
{
    s2_UInt16 rfreq = (s2_UInt16)(1193181/1000);

    /*  https://wiki.osdev.org/Programmable_Interval_Timer 
            Bits         Usage
        6 and 7      Select channel :
                        0 0 = Channel 0
                        0 1 = Channel 1
                        1 0 = Channel 2
                        1 1 = Read-back command (8254 only)
        4 and 5      Access mode :
                        0 0 = Latch count value command
                        0 1 = Access mode: lobyte only
                        1 0 = Access mode: hibyte only
                        1 1 = Access mode: lobyte/hibyte
        1 to 3       Operating mode :
                        0 0 0 = Mode 0 (interrupt on terminal count)
                        0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                        0 1 0 = Mode 2 (rate generator)
                        0 1 1 = Mode 3 (square wave generator)
                        1 0 0 = Mode 4 (software triggered strobe)
                        1 0 1 = Mode 5 (hardware triggered strobe)
                        1 1 0 = Mode 2 (rate generator, same as 010b)
                        1 1 1 = Mode 3 (square wave generator, same as 011b)
        0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
    */
    s2_UInt8 commandFlags = 0b00110110;
    s2_OutB(0x43, commandFlags); // Pit command port;
    s2_OutB(0x40, rfreq & 0xFF); // 0x40 - Counter 0
    s2_OutB(0x40, (rfreq >> 8) & 0xFF);
}

void (*s2_Sleep)() = &s2_PITSleep;

void s2_QEMUDebugPrint(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        s2_OutB(0xE9, str[i]);
        i++;
    }
    s2_OutB(0xE9, '\n');

}
