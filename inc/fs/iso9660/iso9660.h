#pragma once
#include <inc/drivers/ide/ide.h>
#include <inc/types.h>

#define S2_ISO9660LBA_BAD 0xBAD0001

typedef struct
{
    s2_IDEDevice *dev;
    s2_UInt32 rootLBA;
    bool valid;
} s2_ISO9660FS;

typedef struct
{
    s2_UInt32 lba;
    s2_UInt32 size;
} s2_ISO9660DirectoryEntry;


typedef struct
{
    s2_UInt32 size;
    s2_Byte *buf;
} s2_File;

s2_ISO9660FS* s2_ISO9660InstCreate(s2_IDEDevice *dev);
s2_ISO9660DirectoryEntry* s2_ISO9660FSRecursiveDirectoryScan(s2_ISO9660FS *fs, char *path, s2_UInt32 offs);
s2_File* s2_ISO9660ReadFile(s2_ISO9660FS *fs, char *path);
