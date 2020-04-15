#pragma once
#define S2_EPS2KEYBOARD 0
#define S2_EATAPRIMARYIRQ 1
#define S2_EATASECONDARYIRQ 2
#define S2_EPS2KEYBOARDP 3

#define S2_EVENT_QUEUE_MAX 128

#include <inc/mem/manager.h>
#include <inc/types.h>

typedef unsigned int s2_EventType;

typedef struct 
{
    s2_EventType ev_type;
    s2_Time time;
    void *event;
} s2_Event;

typedef struct s2_EventQueueNode_T
{
    struct s2_EventQueueNode_T *next, *prev;
    s2_Event *ev;
} s2_EventQueueNode;


typedef struct
{
    s2_EventQueueNode *rear, *front;
    s2_Size size;
} s2_EventQueue;




// st - Single Threaded
static s2_Event *s2_currentEvent = NULL;
static s2_EventQueue s2_eventQueue;


void s2_EventQueueInit();

void s2_EventQueueQueue(s2_Event *ev);

s2_Event* s2_EventQueueDequeue();

s2_Event* s2_AllocEvent();

void s2_DisposeEvent(s2_Event *event);

s2_Event* s2_EventAwait();

void s2_DispatchEvent(s2_Event *ev);

s2_Event* s2_EventUnqueue();