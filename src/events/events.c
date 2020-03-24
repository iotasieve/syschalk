#include <inc/events/events.h>


s2_Event* s2_EventAwait()
{
    if (s2_currentEvent != NULL) 
    {
        s2_MemoryFree(s2_currentEvent->event);
        s2_DisposeEvent(s2_currentEvent);
        s2_currentEvent = NULL;
    }
     
    while (s2_eventQueue.size == 0)
    {
        asm("hlt");
    }

    if (s2_eventQueue.size > 0)
    {
        s2_currentEvent = s2_EventQueueDequeue();
        return s2_currentEvent;
    }
   
}

void s2_DispatchEvent(s2_Event *ev)
{
    s2_EventQueueQueue(ev);
}

void s2_EventQueueInit()
{
    s2_eventQueue.size = 0;
}

void s2_EventQueueQueue(s2_Event *ev)
{
    asm("cli");
    if (s2_eventQueue.size == 0)
    {
        s2_EventQueueNode *n = (s2_EventQueueNode*)s2_MemoryAlloc(sizeof(s2_EventQueueNode));
        n->ev = ev;
        s2_eventQueue.front = n;
        s2_eventQueue.rear = n;
        s2_eventQueue.size += 1;
    }
    else 
    {
        s2_EventQueueNode *n = (s2_EventQueueNode*)s2_MemoryAlloc(sizeof(s2_EventQueueNode));
        n->next = s2_eventQueue.rear;
        n->next->prev = n;
        s2_eventQueue.rear = n;
        s2_eventQueue.size += 1;
    }
    asm("sti");
}

s2_Event* s2_EventQueueDequeue()
{
    asm("cli");
    if (s2_eventQueue.size > 0)
    {
        s2_EventQueueNode *n = s2_eventQueue.front;
        s2_eventQueue.front = s2_eventQueue.front->prev;
        s2_Event *e = n->ev;
        s2_currentEvent = e;
        s2_eventQueue.size -= 1;
        s2_MemoryFree(n);
        asm("sti");
        return e;
    }
    else 
    {
        asm("sti");
        return NULL;
    }
    asm("sti");
}

s2_Event* s2_AllocEvent()
{
    return s2_MemoryAlloc(sizeof(s2_Event));
}

void s2_DisposeEvent(s2_Event *event)
{
    s2_MemoryFree(event);
}



















