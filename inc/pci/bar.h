#pragma once

#include <inc/types.h>
#define S2_PCI_BARTYPE_MEM
#define S2_PCI_BARTYPE_IO

typedef struct
{
    bool prefetchable;
    s2_Byte *address;
    s2_UInt32 size;
    s2_Byte type; 
} s2_BaseAddressRegister;
