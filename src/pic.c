char in8(short port)
{
    char ret = 0;
    asm volatile("inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

void out8(short port, char data)
{
    asm volatile ( "outb %0, %1" : : "a"(data), "Nd"(port) );
}
