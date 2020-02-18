#include <inc/io.h>

short* VGA_PTR = (short*)0xb8000;

void printvmem(char* str, char color, short offset)
{
    int i = 0;
	while(str[i] != 0)
    { VGA_PTR[i+offset] = (color << 8) | str[i];
        ++i;
    }
}
