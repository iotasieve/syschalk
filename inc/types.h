#pragma once

#define S2_TRUE 1
#define S2_FALSE 0
typedef unsigned char s2_Byte;
typedef unsigned char s2_UInt8;
typedef unsigned short s2_UInt16;
typedef unsigned int s2_UInt32;

typedef unsigned char s2_Int8;
typedef unsigned short s2_Int16;
typedef unsigned int s2_Int32;

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
