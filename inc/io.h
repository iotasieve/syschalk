#ifndef __IO_H
#define __IO_H

#include <inc/types.h>
#define VGA_SCR_WIDTH 80
#define VGA_SCR_HEIGHT 25

#define S2_PANICERR_GENERATED 0
#define S2_PANICERR_MEMALLOC 1


void s2_TVMPrint(char* str, char color, short offset);
void s2_Panic(unsigned short code, char* message, bool isFatal, char* file, char* line);


#endif
