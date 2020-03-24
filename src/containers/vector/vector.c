#include <inc/containers/vector/vector.h>


s2_CtVector* s2_CtVectorCreate(s2_CtVector *r, s2_Size typeSize)
{
    r->data = NEWSZ(typeSize, S2_CTVEC_DEFAULT_CAPACITY);
    r->capacity = S2_CTVEC_DEFAULT_CAPACITY;
    r->size = 0;
    r->typeSize = typeSize;
    return r;
}

void s2_CtVectorPush(s2_CtVector *vec, void* v)
{
    if ((vec->size + 1) > vec->capacity)
    {
        unsigned int pcapacity = vec->capacity;
        vec->capacity += S2_CTVEC_DEFAULT_CAPACITY;
        void* newd = NEWSZ(vec->typeSize, vec->capacity);
        for (int i = 0; i < pcapacity; ++i)
        {
            *ITERPTR(newd, vec->typeSize, i) = *ITERPTR(vec->data, vec->typeSize, i);
        }
        DELETE(vec->data);
        vec->data = newd;
    }
    *ITERPTR(vec->data, vec->typeSize, vec->size) = v;
    vec->size += 1;
}

void* s2_CtVectorPop(s2_CtVector *vec)
{
    if (vec->size < 1)
    {
        return NULL;
    }
    vec->size -= 1;
    return s2_CtVectorIndex(vec, vec->size);
}

inline void* s2_CtVectorIndex(s2_CtVector *vec, s2_Size idx)
{
    return ITERPTR(vec->data, vec->typeSize, idx);
}

void s2_CtVectorDispose(s2_CtVector *vec)
{
    DELETE(vec->data);
}
