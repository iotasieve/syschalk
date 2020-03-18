#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include <inc/events/events.h>
#include <inc/types.h>
#include <inc/pic.h>

#define S2_KESC 0x01
#define S2_KBACKSPACE 0x0e
#define S2_KTAB 0x0f

typedef struct
{
  bool pressed;
  s2_Byte key;
} s2_PS2KeyboardEvent;


void s2_PS2KeyboardHandler();
// s2_Byte s2_PS2KeyboardGetKeyState(s2_Byte KEY);
char s2_PS2ConvertCodeToChar(s2_Byte code);

#endif
