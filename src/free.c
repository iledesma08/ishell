#include "../include/free.h"

extern base;

// Function to merge adjacent free memory blocks
t_block fusion(t_block b)
{
    if (b->next && b->next->free)
    {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;

        if (b->next)
            b->next->prev = b;
    }
    return b;
}

// Function to check if a pointer is valid and part of the heap
int valid_addr(void* p)
{
    if (base)
    {
        if (p > base && p < sbrk(0))
        {
            t_block b = get_block(p);
            return b && (p == b->ptr);
        }
    }
    return (0);
}

// Function to free allocated memory
void free(void* ptr)
{
    t_block b;

    if (valid_addr(ptr))
    {
        b = get_block(ptr);
        b->free = 1;

        if (b->next && b->next->free)
            fusion(b);
        if (b->prev && b->prev->free)
            fusion(b->prev);
        else
        {
            if (b->next)
                b->next->prev = b;
            if (b->prev)
                b->prev->next = b;
            else
                base = b;
            b->free = 1;
            b->prev = NULL;
        }
    }
}