#include <inc/game/texture.h>
#include <inc/types.h>
#include <inc/mem/manager.h>
#include <inc/drivers/vga/vga.h>

void GInitScreen()
{
    gScr = GTexMake(320, 200); 
    gScrIndexed = NEW(s2_UInt8, 320*200);
}

void GScreenText(char *str, int atX, int atY)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
        {
            i++;
            continue;
        }
        int idx = 0;
        while (1)
        {
            if ((str[i] == gFontmap[idx]) || (str[i] == gAltfontmap[idx]))
            {
                break;
            }
            if (gFontmap[idx] == '\0')
            {
                idx = 39;
                break;
            }
            idx += 1;
        }

        GScreenBlitB(gFont, atX+(i*8), atY, idx*8, 0, 8, 8);
        i++;
    }
}

void GFontInit(GTexture *tex, char *fontmap, char *altfontmap)
{
    gFont = tex;
    fontmap = fontmap;
    gAltfontmap = altfontmap;
    gFontmap = fontmap;
}

GTexture *GTexMake(int width, int height)
{
    GTexture *tex = s2_MemoryAlloc(sizeof(GTexture));
    tex->width = width;
    tex->height = height;
    tex->buffer = NEW(int, width*height);
    return tex;
}

inline s2_UInt8 *GTexIndexColor(int color)
{
    s2_UInt8 r = (color >> 16) & 0xFF;
    s2_UInt8 g = (color >> 8) & 0xFF;
    s2_UInt8 b = (color) & 0xFF;
    
    return ((r/51)*6*6)+((g/51)*6)+(b/51)+0b10000;
}

s2_UInt8* GScreenGenIndexed()
{

    for (int i = 0; i < gScr->width; i++)
        for (int j = 0; j < gScr->height; j++)
            {
                gScrIndexed[i + j * gScr->width] = GTexIndexColor(GTexMap(gScr, i, j));

            }
    return gScrIndexed;
}

void GScreenClear(int color)
{
    for (int i = 0; i < gScr->width; i++)
        for (int j = 0; j < gScr->height; j++)
        {
            // s2_QEMUDebugPrint(s2_ToHex(*GTexMap(gScr, i, j)));
            GTexMap(gScr, i, j) = color;
        }
}

void GScreenBlitB(GTexture *tex, int atX, int atY, int ix, int iy, int iw, int ih)
{
    if ((atX) > gScr->width) return; 
    if ((atY) > gScr->height) return; 
    if ((atX+iw) < 0) return; 
    if ((atY+ih) < 0) return; 
    for (int i = 0; i < iw; i++)
        for (int j = 0; j < ih; j++)
        {
            if ((i+atX) < 0) continue;
            if ((j+atY) < 0) continue;
            if ((i+atX) >= gScr->width) continue;
            if ((j+atY) >= gScr->height) continue;
            if ((i+ix) < 0) continue;
            if ((j+iy) < 0) continue;
            if ((i+ix) >= tex->width) continue;
            if ((j+iy) >= tex->height) continue;
            if (!((GTexMap(tex, i+ix, j+iy) >> 24) & 0xFF)) continue;
            GTexMap(gScr, i+atX, j+atY) = GTexMap(tex, i+ix, j+iy);
        }
}

void GScreenBlit(GTexture *tex, int atX, int atY)
{
    for (int i = 0; i < tex->width; i++)
        for (int j = 0; j < tex->height; j++)
        {
            if ((i+atX) < 0) continue;
            if ((j+atY) < 0) continue;
            if ((i+atX) >= gScr->width) continue;
            if ((j+atY) >= gScr->height) continue;
            if (!((GTexMap(tex, i, j) >> 24) & 0xFF)) continue;
            GTexMap(gScr, i+atX, j+atY) = GTexMap(tex, i, j);
        }
}

void GScreenRect(int atX, int atY, int width, int height, int color)
{
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            if ((i+atX) < 0) continue;
            if ((j+atY) < 0) continue;
            if ((i+atX) >= gScr->width) continue;
            if ((j+atY) >= gScr->height) continue;
            GTexMap(gScr, i+atX, j+atY) = color;
        }
}

void GScreenDraw()
{
    s2_VGADrawBuffer8I(GScreenGenIndexed(gScr));
}