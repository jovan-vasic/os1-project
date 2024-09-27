#ifndef _MEMORYALLOCATOR_HPP
#define _MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct MemBlock {
    MemBlock *next, *prev;
    size_t size;
};

class MemoryAllocator {
private:
    static MemBlock *freeMem;

public:
    static void initMem();

    static void *mem_alloc(size_t );

    static int mem_free(void *);

    static void tryToJoin(MemBlock *); // helper
};

#endif
