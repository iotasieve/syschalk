#include <inc/events/events.h>
#include <inc/io.h>
s2_Event* s2_EventAwait()
{
    asm("hlt");
    return s2_currentEvent;
}

void s2_DispatchEvent(s2_Event *ev)
{
    s2_currentEvent = ev;
}







