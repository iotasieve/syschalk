#include <inc/io.h>

// Forward delcare
void* s2_MemoryAlloc(s2_Size size);

bool lolmode = false;
short *VGA_PTR = (short *)0xb8000;

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
    s2_TVMPrint("at ", 0x04, 240);
    s2_TVMPrint(line, 0x04, 243);
    
    switch (code)
    {
    case S2_PANICERR_GENERATED:
        s2_TVMPrint("S2_PANICERR_GENERATED", 0x04, 480);
        break;
    case S2_PANICERR_MEMALLOC:
        s2_TVMPrint("S2_PANICERR_MEMALLOC", 0x04, 480);
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

void s2_TVMClear(char color)
{
    s2_TVMPrint("                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ", color, 0);
}