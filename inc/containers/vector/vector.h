#pragma once
#include <inc/types.h>
#include <inc/mem/manager.h>
#define S2_CTVEC_DEFAULT_CAPACITY 10000
typedef struct
{
    void *data;
    s2_Size size, typeSize, capacity;
} s2_CtVector;

s2_CtVector* s2_CtVectorCreate(s2_CtVector *r, s2_Size typeSize);

void s2_CtVectorPush(s2_CtVector *vec, void* v);

void* s2_CtVectorPop(s2_CtVector *vec);

void* s2_CtVectorIndex(s2_CtVector *vec, s2_Size idx);

void s2_CtVectorDispose(s2_CtVector *vec);