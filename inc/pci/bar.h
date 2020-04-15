#pragma once

#include <inc/types.h>
#define S2_PCI_BARTYPE_MEM 0
#define S2_PCI_BARTYPE_IO 1

typedef struct
{
    bool prefetchable;
    s2_UInt8 *address;
    s2_UInt32 size;
    s2_Byte type; 
} s2_BaseAddressRegister;
