#include <inc/drivers/ps2_keyboard.h>
static s2_Byte kout = 0;
static const char* scancode_chr = "1234567890-=qwertyuiop[]\nasdfghjkl;'`\\zxcvbnm,./";

void s2_PS2KeyboardHandler()
{
    s2_PS2KeyboardEvent *pe = (s2_PS2KeyboardEvent*)s2_MemoryAlloc(sizeof(s2_PS2KeyboardEvent));

    kout = s2_InB(0x60);
    if (kout < 0x81)
    {
        pe->pressed = true;
        pe->key = kout;
    }
    else
    {
        pe->pressed = false;
        pe->key = kout-0x80;
    }
    s2_Event *e = s2_AllocEvent();
    e->ev_type = S2_EPS2KEYBOARD;
    e->time = 0;
    e->event = pe;
    s2_DispatchEvent(e);
}

char s2_PS2ConvertCodeToChar(s2_Byte key)
{
    if (key > 0x01 && key < 0x0e)
    {
        return scancode_chr[key - 0x2];
    }
    else if (key > 0x0f && key < 0x1d)
    {
        return scancode_chr[key - 0x10 + 12];
    }
    else if (key > 0x1d && key < 0x2a)
    {
        return scancode_chr[key - 0x1e + 25];
    }
    else if (key > 0x2a && key < 0x36)
    {
        return scancode_chr[key - 0x2b + 37];
    }
    else if (key == 0x39) return ' ';
    return key;
}