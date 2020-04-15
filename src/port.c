#include <inc/port.h>

inline s2_Byte s2_InB(short port)
{
    char ret = 0;
    asm volatile("inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

inline void s2_OutB(short port, s2_Byte data)
{
    asm volatile ( "outb %0, %1" : : "a"(data), "Nd"(port) );
}

inline long s2_InL(short port)
{   
    long ret = 0;
    asm volatile("inl %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

inline void s2_OutL(short port, long data)
{
    asm volatile ( "outl %0, %1" : : "a"(data), "Nd"(port) );
}

inline s2_UInt16 s2_InW(short port)
{
    s2_UInt16 ret = 0;
    asm volatile("inw %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;   
}

inline void s2_OutW(short port, s2_UInt16 data)
{
    asm volatile ( "outw %0, %1" : : "a"(data), "Nd"(port) );
}
