#include <inc/fs/iso9660/iso9660.h>


s2_ISO9660FS* s2_ISO9660InstCreate(s2_IDEDevice *dev)
{
    s2_ISO9660FS *fs = NEW(s2_ISO9660FS, 1);
    fs->dev = dev;
    s2_Byte *buf = s2_MemoryAlloc(S2_ATAPI_SECTOR_SIZE);
    s2_IDEATAPIRead(dev, 16, buf, 2048);

    // TODO: More valid cd check
    if (*(buf+1) != 'C')
    {
        return fs;
    } 
    s2_UInt32 rootLBA = *((s2_UInt32*)((buf+158)));
    s2_MemoryFree(buf);
    fs->rootLBA = rootLBA;
    fs->valid = true;
    return fs;
}

s2_ISO9660DirectoryEntry* s2_ISO9660FSRecursiveDirectoryScan(s2_ISO9660FS *fs, char *path, s2_UInt32 offs)
{
    s2_Byte *bufo = s2_MemoryAlloc(S2_ATAPI_SECTOR_SIZE);
    s2_Byte *buf = bufo;
    s2_IDEATAPIRead(fs->dev, offs, buf, 2048);

    while (*buf != 0x00)
    {
        s2_Byte namelen = *(buf+32);
        char *nameloc = (buf+33);
        bool samepath = true;
        bool final = false;
        char iter = 0;
        s2_Byte pathlen = 0;
        while (true)
        {
            if ((path[iter] == '/') || (path[iter] == '\0')) break;
            pathlen += 1;
            iter += 1;
        }
        if ((pathlen != namelen) && (pathlen != (namelen-2))) 
        {
            buf += *buf;
            continue;
        }
        for (int i = 0; i < namelen; i++)
        {
            if (nameloc[i] == ';') {
                final = true;
                break;
            }
            else if (nameloc[i] != path[i])
            {
                samepath = false;
            }
        }
        if (!samepath)
        {
            buf += *buf;
            continue;
        }
        else {

            s2_MemoryFree(bufo);
            if (final)
            {
                s2_ISO9660DirectoryEntry *dir = NEW(s2_ISO9660DirectoryEntry, 1);
                dir->lba = *((s2_UInt32*)((buf+2)));
                dir->size = *((s2_UInt32*)((buf+10)));
                return dir;
            }
            else
            {
                s2_ISO9660DirectoryEntry *dir = s2_ISO9660FSRecursiveDirectoryScan(fs, path+pathlen+1, *((s2_UInt32*)((buf+2))));

                return dir;
            }
        }
    }
    s2_ISO9660DirectoryEntry *dir = NEW(s2_ISO9660DirectoryEntry, 1);
    dir->lba = S2_ISO9660LBA_BAD;
    dir->size = 0;
    return dir;
}

s2_File* s2_ISO9660ReadFile(s2_ISO9660FS *fs, char *path)
{
    s2_File *res = NEW(s2_File, 1);

    s2_TVMPrintA(path);
    s2_ISO9660DirectoryEntry *dir = s2_ISO9660FSRecursiveDirectoryScan(fs, path, fs->rootLBA);
    if (dir->lba == S2_ISO9660LBA_BAD) 
    {
        res->size = 0;
        res->buf = NULL;
        return res;
    }
    s2_UInt32 secsz = (dir->size/2048);

    // s2_QEMUDebugPrint("SEcz");
    if ((dir->size-(2048*secsz)) > 0) secsz += 1;

    s2_Byte *buf = s2_MemoryAlloc(dir->size);

    for (int i = 0; i < secsz; i++)
    {
        s2_IDEATAPIRead(fs->dev, dir->lba+i, buf+(2048*i), (dir->size-(2048*(i))));
        // s2_QEMUDebugPrint(s2_ToHex(buf));
    }
    
    res->buf = buf;

    return res;
}