#include <inc/mem/manager.h>

unsigned int heap_start = 10*1024*1024;

void s2_InitMemoryAllocator()
{
    rootEntry = (s2_MemoryEntry*)heap_start;
    rootEntry->size = 512; // To be fair anything would do
    rootEntry->prev = NULL;
    rootEntry->next = NULL;
    rootEntry->flags = S2_MEMFLAG_DEFAULT;
}

/**
 *  @param entry the entry to bootstrap from
 *  @param desiredSize size desired to get after allocation
 *  @returns false if operation failed, true if operation succeeded. The memory chunk is passed via param hence can be used if operation succeeded 
 */
bool s2_MemoryMergeIfPossible(s2_MemoryEntry *entry, s2_Size desiredSize)
{
    s2_MemoryEntry *eIter = entry;
    while ((eIter->next->flags & S2_MEMFLAG_ISALLOC) == false)
    {
        eIter = eIter->next;

        // Check
        entry->next = eIter;
        entry->size = (unsigned int)(entry-eIter)-sizeof(s2_MemoryEntry);

        if (entry->size >= desiredSize) return true;
            
    }
    return false;
}
/**
 * @param size Desired size
 * @returns pointer
 */
void* s2_MemoryAlloc(s2_Size size)
{
    asm("cli");
    
    s2_Size memory_cap_loc = heap_start+S2_MEMCAP;
    s2_MemoryEntry *biggestFree = NULL; 
    s2_MemoryEntry *lastEntry;
    for (s2_MemoryEntry *entry = rootEntry; entry != 0; entry = entry->next)
    {
        lastEntry = entry;
        if (entry->size >= size && ((entry->flags & S2_MEMFLAG_ISALLOC) == false))
        {
            entry->flags = S2_MEMFLAG_ISALLOC;
            asm("sti");
            return entry+1;
        }
        if (biggestFree != NULL && entry->size >= biggestFree->size && ((entry->flags & S2_MEMFLAG_ISALLOC) == false))
        {
            biggestFree = entry; 
        }
        else if (biggestFree == NULL && ((entry->flags & S2_MEMFLAG_ISALLOC) == false)) biggestFree = entry;
    }
    if (biggestFree && s2_MemoryMergeIfPossible(biggestFree, size))
    {
        biggestFree->flags = S2_MEMFLAG_ISALLOC;
        asm("sti");
        return biggestFree+1;
    }
    else {
        // Create new entry
        if ((s2_Size)(lastEntry+lastEntry->size) > memory_cap_loc) return NULL;
        if ((s2_Size)(lastEntry+lastEntry->size+size) > memory_cap_loc) return NULL;
        
        s2_MemoryEntry *newEntry = (s2_MemoryEntry*)((s2_Byte*)lastEntry+lastEntry->size+sizeof(s2_MemoryEntry));
        newEntry->size = size;
        newEntry->prev = lastEntry;
        newEntry->next = NULL;
        newEntry->flags = S2_MEMFLAG_ISALLOC;
        lastEntry->next = newEntry;
        
        asm("sti");
        return newEntry+1;

    }
    asm("sti");
    // Just in case
    return NULL;
}

void s2_MemoryFree(void *ptr)
{
    ptr = (s2_Byte*)ptr;
    ((s2_MemoryEntry*)(ptr-sizeof(s2_MemoryEntry)))->flags &= ~S2_MEMFLAG_ISALLOC;
}

void s2_MemoryPurge(void *ptr)
{
    ptr = (s2_Byte*)ptr;
    s2_MemoryEntry *eptr = ((s2_MemoryEntry*)(ptr-sizeof(s2_MemoryEntry)));
    for (s2_Size i = 0; i < (eptr->size); i++)
    {
        *(char*)(i+&ptr) = (char)0x69; // le funi number (debug, will be changed to 0)
    }
    s2_MemoryFree(ptr);
}

inline void* s2_MemoryCopy(void *dest, void *src, s2_Size size)
{
    for (int i = 0; i < (size); i++)
        *((s2_UInt8*)(dest)+i) = *((s2_UInt8*)(src)+i);
    return dest;
}