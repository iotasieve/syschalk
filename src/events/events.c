#include <inc/events/events.h>

int eventLoss;


s2_Event* s2_EventAwait()
{
    if (s2_currentEvent != NULL) 
    {
        // s2_DisposeEvent(s2_currentEvent);
        // s2_MemoryFree(s2_currentEvent);
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

s2_Event* s2_EventUnqueue()
{
    if (s2_currentEvent != NULL) 
    {
        // s2_DisposeEvent(s2_currentEvent);
        // s2_MemoryFree(s2_currentEvent);
        s2_currentEvent = NULL;
    }

    s2_currentEvent = s2_EventQueueDequeue();
    return s2_currentEvent;
}


void s2_DispatchEvent(s2_Event *ev)
{
    eventLoss += 1;
    s2_EventQueueQueue(ev);
}

void s2_EventQueueInit()
{
    eventLoss = 0;
    s2_eventQueue.front = 0;
    s2_eventQueue.rear = 0;
    s2_eventQueue.size = 0;
}

void s2_EventQueueQueue(s2_Event *ev)
{
    s2_EventQueueNode *n = (s2_EventQueueNode*)s2_MemoryAlloc(sizeof(s2_EventQueueNode));
    n->ev = ev;
    

    if (s2_eventQueue.size == 0)
    {
        s2_eventQueue.front = n;
    }
    else 
    {
        s2_eventQueue.rear->prev = n;
        n->next = s2_eventQueue.rear;
        s2_eventQueue.rear = n;
    }
    s2_eventQueue.size += 1;
}

s2_Event* s2_EventQueueDequeue()
{
    if (s2_eventQueue.size > 0)
    {
        if (s2_eventQueue.size == 2)
        {
            s2_eventQueue.front->prev = s2_eventQueue.rear;
        }
        s2_EventQueueNode *n = s2_eventQueue.front;
        s2_eventQueue.front = s2_eventQueue.front->prev;
        s2_Event *e = n->ev;
        s2_eventQueue.size -= 1;
        // s2_MemoryFree(n);
        return e;
    }
    return NULL;
}

s2_Event* s2_AllocEvent()
{
    return s2_MemoryAlloc(sizeof(s2_Event));
}

void s2_DisposeEvent(s2_Event *event)
{
    s2_MemoryFree(event->event);
}



















