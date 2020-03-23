#pragma once
#define S2_EKEYBOARD 0

#include <inc/types.h>

typedef unsigned int s2_EventType;

typedef struct {
    s2_EventType ev_type;
    s2_Time time;
    void *event;
} s2_Event;

// st - Single Threaded
static bool s2_stEventWaiting = false;
static s2_Event *s2_currentEvent;

s2_Event* s2_EventAwait();
void s2_DispatchEvent(s2_Event *ev);
