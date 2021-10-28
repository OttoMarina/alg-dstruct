#include <stdio.h>
#include "memallocator.h"

void* head = NULL;
void* beginningOfHeap = NULL;
void* nextSuitable = NULL;
int size = 0;

int memgetminimumsize() {
    return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int memgetblocksize() {
    return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int* LeftSizeBlock(void* block) {
    return (int*)block;
}

void** NextBlock(void* block) {
    return (void**)((char*)block + sizeof(int));
}

void** PreviousBlock(void* block) {
    return (void**)((char*)block + sizeof(int) + sizeof(void*));
}

int* RightSizeBlock(void* block) {
    return (int*)((char*)block + *LeftSizeBlock(block) - sizeof(int));
}

int meminit(void* pMemory, int size) {
    if (size <= memgetminimumsize())
        return 0;
    void* block = pMemory;
    *LeftSizeBlock(block) = size;
    *NextBlock(block) = NULL;
    *PreviousBlock(block) = NULL;
    *RightSizeBlock(block) = size;
    head = block;
    beginningOfHeap = pMemory;
    size = size;
    nextSuitable = head;
    return size;
}

void memdone() {
    void* block = beginningOfHeap;
    while ((char*)block < (char*)beginningOfHeap + size) {
        if (*RightSizeBlock(block) < 0)
            fprintf(stderr, "[MEMORY LEAK] block at 0x%p\n", (char*)block + sizeof(int) + sizeof(void*) + sizeof(void*));
        block = (void*)((char*)block + *LeftSizeBlock(block));
    }
}

void* memalloc(int size) {
    if (size < 1 || size > size - memgetblocksize())
        return NULL;
    void* suitBlock = nextSuitable;
    int found = 0;
    void* freeBlock = NULL;
    void* prev = NULL;
    while (found != 1 && suitBlock != NULL) {
        if (*LeftSizeBlock(suitBlock) >= memgetblocksize() + size) {
            found = 1;
        }
        prev = suitBlock;
        suitBlock = *NextBlock(suitBlock);
        if (found == 0 && nextSuitable != head && suitBlock == NULL)
            suitBlock = head;
        if (suitBlock == nextSuitable)
            break;
    }
    if (found == 0) {
        return NULL;
    }
    suitBlock = prev;
    if (*LeftSizeBlock(suitBlock) > memgetblocksize() + size + memgetblocksize()) {
        freeBlock = (void*)((char*)suitBlock + memgetblocksize() + size);
        *LeftSizeBlock(freeBlock) = *LeftSizeBlock(suitBlock) - memgetblocksize() - size;
        *NextBlock(freeBlock) = *NextBlock(suitBlock);
        *PreviousBlock(freeBlock) = *PreviousBlock(suitBlock);
        *RightSizeBlock(freeBlock) = *LeftSizeBlock(freeBlock);
        if (*PreviousBlock(suitBlock) != NULL)
            *NextBlock(*PreviousBlock(suitBlock)) = freeBlock;
        else
            head = freeBlock;
        if (*NextBlock(suitBlock) != NULL)
            *PreviousBlock(*NextBlock(suitBlock)) = freeBlock;
        *LeftSizeBlock(suitBlock) = memgetblocksize() + size;
    }
    else {
        if (*PreviousBlock(suitBlock) != NULL) {
            *NextBlock(*PreviousBlock(suitBlock)) = *NextBlock(suitBlock);
            if (*NextBlock(suitBlock) != NULL)
                *PreviousBlock(*NextBlock(suitBlock)) = *PreviousBlock(suitBlock);
        }
        else {
            head = *NextBlock(suitBlock);
            if (head != NULL)
                *PreviousBlock(head) = NULL;
        }
    }
    if (*NextBlock(suitBlock) != NULL)
        nextSuitable = *NextBlock(suitBlock);
    else
        nextSuitable = head;
    *RightSizeBlock(suitBlock) = -*LeftSizeBlock(suitBlock);
    *NextBlock(suitBlock) = NULL;
    *PreviousBlock(suitBlock) = NULL;
    return (void*)((char*)suitBlock + sizeof(int) + sizeof(void*) + sizeof(void*));
}


void memfree(void* p) {
    void* futureFree = (void*)((char*)p - sizeof(void*) - sizeof(int) - sizeof(void*));
    void* leftBlock = NULL;
    void* rightBlock = NULL;
    int mergedWithLeft = 0;
    int mergedWithRight = 0;
    if ((char*)futureFree - 1 > (char*)beginningOfHeap)
        leftBlock = (void*)((char*)futureFree - (*((int*)futureFree - 1) >= 0 ? *((int*)futureFree - 1) : -*((int*)futureFree - 1)));
    if ((char*)futureFree + *LeftSizeBlock(futureFree) + 1 < (char*)beginningOfHeap + size)
        rightBlock = (void*)(RightSizeBlock(futureFree) + 1);
    *RightSizeBlock(futureFree) = -*RightSizeBlock(futureFree);
    if (leftBlock != NULL && *RightSizeBlock(leftBlock) > 0) {
        mergedWithLeft = 1;
        *LeftSizeBlock(leftBlock) += *LeftSizeBlock(futureFree);
        *RightSizeBlock(leftBlock) = *LeftSizeBlock(leftBlock);
        futureFree = leftBlock;
    }
    if (rightBlock != NULL && *RightSizeBlock(rightBlock) > 0) {
        mergedWithRight = 1;
        if (mergedWithLeft == 0) {
            *NextBlock(futureFree) = head;
            *PreviousBlock(head) = futureFree;
            *PreviousBlock(futureFree) = NULL;
            head = futureFree;
        }
        if (rightBlock == head) {
            head = *NextBlock(rightBlock);
            *PreviousBlock(head) = NULL;
        }
        else {
            *NextBlock(*PreviousBlock(rightBlock)) = *NextBlock(rightBlock);
            if (*NextBlock(rightBlock) != NULL)
                *PreviousBlock(*NextBlock(rightBlock)) = *PreviousBlock(rightBlock);
        }
        *LeftSizeBlock(futureFree) += *LeftSizeBlock(rightBlock);
        *RightSizeBlock(futureFree) = *LeftSizeBlock(futureFree);
        if (nextSuitable == rightBlock)
            nextSuitable = futureFree;
    }
    if (mergedWithLeft == 0 && mergedWithRight == 0) {
        *NextBlock(futureFree) = head;
        if (head != NULL)
            *PreviousBlock(head) = futureFree;
        else
            nextSuitable = futureFree;
        *PreviousBlock(futureFree) = NULL;
        head = futureFree;
    }
}