#include <inc/game/bmp.h>

GTexture* GReadBMPFile(char *path)
{
    
    s2_File *f = s2_ISO9660ReadFile(gFS, path);
    
    s2_UInt32 width = *((s2_UInt32*)(f->buf+18));
    s2_UInt32 height = *((s2_UInt32*)(f->buf+22));
    if ((width < 4000) && (height < 4000))
    {
        s2_UInt32 *texloc = f->buf+*((s2_UInt32*)(f->buf+10));
        GTexture *res = GTexMake(width, height);
        
        for (int i = 0; i < width*height; i++)
        {
            GTexMap(res, i%width, (height-1)-(i/width)) = *(texloc+i);
        }
        s2_MemoryFree(f);
        return res;
    }
    s2_MemoryFree(f);
    return NULL;
}