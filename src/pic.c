char s2_In8(short port)
{
    char ret = 0;
    asm volatile("inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

void s2_Out8(short port, char data)
{
    asm volatile ( "outb %0, %1" : : "a"(data), "Nd"(port) );
}