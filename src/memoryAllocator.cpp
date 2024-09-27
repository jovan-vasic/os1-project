#include "../lib/hw.h"
#include "../h/memoryAllocator.hpp"

MemBlock* MemoryAllocator::freeMem = nullptr; // Initialization

void MemoryAllocator::initMem() {
    freeMem = (MemBlock*) HEAP_START_ADDR;
    if (freeMem) {
        freeMem->next = freeMem->prev = nullptr;
        freeMem->size = (char*) HEAP_END_ADDR - (char*) HEAP_START_ADDR - sizeof(MemBlock);
    }
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size == 0 || freeMem == nullptr) return nullptr;

    // Align size to MEM_BLOCK_SIZE
    size = (size + MEM_BLOCK_SIZE - 1) & ~(MEM_BLOCK_SIZE - 1);

    MemBlock* current = freeMem;
    MemBlock* prev = nullptr;

    // Find a suitable block
    while (current != nullptr && current->size < size + sizeof(MemBlock)) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) return nullptr;

    MemBlock* newBlock = nullptr;
    if (current->size > size + sizeof(MemBlock)) {
        // Fragment the current block
        newBlock = (MemBlock*)((char*)current + size + sizeof(MemBlock));
        newBlock->size = current->size - size - sizeof(MemBlock);
        newBlock->next = current->next;
        if (newBlock->next != nullptr) {
            newBlock->next->prev = newBlock;
        }
        current->size = size + sizeof(MemBlock);
    } else {
        // Do not fragment the current block
        newBlock = current->next;
    }

    // Relink
    if (prev != nullptr) {
        prev->next = newBlock;
    } else {
        freeMem = newBlock;
    }
    if (newBlock != nullptr) {
        newBlock->prev = prev;
    }

    return (void*)((char*)current + sizeof(MemBlock));
}

int MemoryAllocator::mem_free(void *ptr) {
    if (ptr == nullptr) return -1;

    char *charPtr = (char*)ptr;
    if (charPtr - sizeof(MemBlock) < HEAP_START_ADDR || charPtr + MEM_BLOCK_SIZE > HEAP_END_ADDR) return -1;

    MemBlock *blk = (MemBlock*)(charPtr - sizeof(MemBlock));
    MemBlock *current = freeMem;

    // Case 1: blk should be inserted before the first free block
    if ((char*)current >= (char*)blk + blk->size) {
        blk->next = current;
        blk->prev = nullptr;
        if (current != nullptr) {
            current->prev = blk;
        }
        freeMem = blk;
        tryToJoin(blk);
        return 0;
    }

    // Case 2: blk should be inserted somewhere after the first free block
    while (current != nullptr) {
        if ((char*)current + current->size <= (char*)blk &&
            (current->next == nullptr || (char*)current->next >= (char*)blk + blk->size)) {
            blk->next = current->next;
            blk->prev = current;
            if (current->next != nullptr) {
                current->next->prev = blk;
            }
            current->next = blk;
            tryToJoin(current);
            tryToJoin(blk);
            return 0;
        }
        current = current->next;
    }

    return -2;
}

// Merges `blk` with `blk->next` if they are adjacent
void MemoryAllocator::tryToJoin(MemBlock* blk) {
    if (blk != nullptr && blk->next != nullptr) {
        // Check if `blk` is immediately adjacent to `blk->next`
        if ((char*)blk + blk->size == (char*)blk->next) {
            // Merge `blk` and `blk->next`
            blk->size += blk->next->size;
            blk->next = blk->next->next;

            // Update the `prev` pointer of the new `blk->next` if it exists
            if (blk->next != nullptr) {
                blk->next->prev = blk;
            }
        }
    }
}