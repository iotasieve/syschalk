#pragma once
#include <inc/types.h>

extern s2_UInt8 s2_InB(short port);
extern void s2_OutB(short port, s2_Byte data);

extern s2_UInt16 s2_InW(short port);
extern void s2_OutW(short port, s2_UInt16 data);

extern long s2_InL(short port);
extern void s2_OutL(short port, long data);
