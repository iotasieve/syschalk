#pragma once

#include <inc/types.h>

#define VGA_SCR_WIDTH 80
#define VGA_SCR_HEIGHT 25

#define S2_PANICERR_GENERATED 0
#define S2_PANICERR_MEMALLOC 1
#define S2_PANICERR_NOVGASUPPORT 3

extern bool lolmode;
void s2_TVMPrint(char* str, char color, short offset);
void s2_Panic(unsigned short code, char* message, bool isFatal, char* file, char* line);
char* s2_ToHex(unsigned int num);
void s2_TVMClear(char color);