#define VGA_SCR_WIDTH 80
#define VGA_SCR_HEIGHT 25
short* VGA_PTR = (short*)0xb8000;


void printvmem(const char* str, char color, short offset)
{
    int i = 0;
	while(str[i] != 0)
    {
        VGA_PTR[i+offset] = (color << 8) | str[i];
        ++i;
    }
}


// TODO: CHANGE THIS
void cleanscr(char color)
{
    printvmem("                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ", color, 0);
}

void draw_circle(int r, char color)
{
    for (int i = 0; i < (2*r); i++)
    {
        for (int j = 0; j < (2*r); j++)
        {
            int d = (i*i)+(j*j);
            if ((d+90) < r*r)
            {   
                printvmem("#", color, i+(80*j));
            }
            else if (d < r*r)
            {
                printvmem("#", ((color & 0x0F) | 0x3F), i+(80*j));
            }
        }
    }
}

void sleep(short millis)
{
    for (int i = 0; i < millis * 100000; ++i) {}
}

extern void kmain()
{
    const short str_l = 39;
    short offset = 0;
	const char* hello = "WELCOME TO ISHIDEX2'S OPERATING SYSTEM!";
    while (1) 
    {
        cleanscr(0x33);
        draw_circle(offset >> 4, 0xFF);
        printvmem(hello, 0x21, offset);
        printvmem("EPILEPSY WARNING, NO DOUBLE BUFFERING", 0xF4, 80-37);
        printvmem("v0.0.1a", 0x21, 2000-7);
        printvmem("IT CAN DO ABSOLUTELY NOTHING!", 0x21, 2000-80);
        if (offset > (2000-str_l))
        {
            offset = 0;
        }
        offset += 1;
        sleep(100);
    }
}
