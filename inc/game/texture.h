#pragma once
#include <inc/types.h>
#include <inc/io.h>
#define GTexMap(tex, x, y) tex->buffer[(x)+(y)*tex->width]

typedef struct {
    int *buffer;
    int width;
    int height; 
} GTexture;

s2_UInt8 *gScrIndexed;
GTexture *gScr;
GTexture *gFont;
char *gFontmap;
char *gAltfontmap;

void GFontInit(GTexture *tex, char *fontmap, char *altfontmap);
void GInitScreen();
GTexture *GTexMake(int width, int height);
void GTexBlit(GTexture *tex);
void GScreenText(char *str, int atX, int atY);
s2_UInt8 *GScreenGenIndexed();
extern s2_UInt8 *GTexIndexColor(int color);
void GScreenBlitB(GTexture *tex, int atX, int atY, int ix, int iy, int iw, int ih);
void GScreenClear(int color);
void GScreenBlit(GTexture *tex, int atX, int atY);
void GScreenRect(int atX, int atY, int width, int height, int color);
void GScreenDraw();