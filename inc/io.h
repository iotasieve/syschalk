#pragma once

#include <inc/types.h>

#define VGA_SCR_WIDTH 80
#define VGA_SCR_HEIGHT 25

#define S2_PANICERR_GENERATED 0
#define S2_PANICERR_MEMALLOC 1
#define S2_PANICERR_NOVGASUPPORT 3

extern bool lolmode;
extern s2_UInt32 s2_IOPITCountdown;
char* s2_ToDec(unsigned int num);
void s2_TVMPrint(char* str, char color, short offset);
void s2_Panic(unsigned short code, char* message, bool isFatal, char* file, char* line);
char* s2_ToHex(unsigned int num);
void s2_TVMClear(char color);
extern void s2_IOWait(void);
void s2_TVMPrintA(char *str);
void s2_PITSleep(s2_UInt32 ms);
void s2_PITTimerInit();
extern void (*s2_Sleep)();
void s2_QEMUDebugPrint(char *str);
