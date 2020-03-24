#pragma once

#define S2_TRUE 1
#define S2_FALSE 0
typedef unsigned char s2_Byte;
typedef unsigned int s2_Time;

#define NULL 0
#define bool char
#define true 1
#define false 0
#define ITERPTR(ptr, step, idx) (((s2_Byte*)(ptr))+((step)*(idx)))
#define NEW(type, len) (type*)s2_MemoryAlloc(sizeof(type)*(len))
// New by specified size
#define NEWSZ(typesz, len) s2_MemoryAlloc((typesz)*(len))
#define DELETE(ptr) s2_MemoryFree(ptr)
#define OF(type) sizeof(type)

typedef unsigned int s2_Size;
