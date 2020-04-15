#include <inc/game/texgen.h>
#include <inc/mem/manager.h>

// int GGimpHTexGetPixel(GGimpImg *img, int x, int y)
// {
    // img->
// }

GTexture* GGenWoodTile()
{
    GTexture *tex = GTexMake(16, 16);

    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
        {
            GTexMap(tex, i, j) = 0xFFffa34d;
            if ((j % 4) == 3) GTexMap(tex, i, j) = 0xFF6b3f16;
        }
    
    return tex;
}

GTexture* GGenPlayerFront()
{
    char pixel[3];
    GTexture *tex = GTexMake(16, 128);

    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 128; j++)
        {
            // GTexMap(tex, i, j) = GPLAYERIDLE_HEADER_PIXEL()
        }
    
    return tex; 
}

GTexture* GGenDoorOpenTile()
{
    GTexture *tex = GTexMake(16, 16);

    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
        {
            GTexMap(tex, i, j) = 0x0; // Transparent
            if (i > 13) GTexMap(tex, i, j) = 0xFF6b3f16;
            if ((i > 12) && (j<14) && (j > 1)) GTexMap(tex, i, j) = 0xFF6b3f16;
        }
    
    return tex;
}

GTexture* GGenDoorTile()
{
    GTexture *tex = GTexMake(16, 16);

    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
        {
            GTexMap(tex, i, j) = 0xFF6b3f16;
            if ((j >= 2) && (j < 10) && (i >= 2) && (i < 14)) GTexMap(tex, i, j) = 0x0; // transparent
            if (i == 0) GTexMap(tex, i, j) = 0xFFffa34d;
            if (j == 0) GTexMap(tex, i, j) = 0xFFffa34d;
        }
    
    return tex;
}

GTexture* GGenFloorTile()
{
    GTexture *tex = GTexMake(16, 16);

    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
        {
            GTexMap(tex, i, j) = 0xFFdedede;
            if (i == 0) GTexMap(tex, i, j) = 0xFFffffff;
            if (j == 0) GTexMap(tex, i, j) = 0xFFffffff;
            if (i == 15) GTexMap(tex, i, j) = 0xFFa8a8a8;
            if (j == 15) GTexMap(tex, i, j) = 0xFFa8a8a8;
        }
    
    return tex;
}