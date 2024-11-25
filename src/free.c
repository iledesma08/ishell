#include "../include/free.h"

extern base;

// Function to merge adjacent free memory blocks
t_block fusion(t_block b)
{
    if (b->next && b->next->free)
    {                                          // Check if the next block is free
        b->size += BLOCK_SIZE + b->next->size; // Merge the sizes
        b->next = b->next->next;               // Skip the merged block
    }
    if (b->next)
    {
        b->next->prev = b; // Update the previous pointer of the next block
    }
    return b;
}

// Function to check if a pointer is valid and part of the heap
int valid_addr(void* p)
{
    if (base)
    { // Ensure the heap exists
        if (p > base && p < sbrk(0))
        {                                    // Check if the pointer is within the heap range
            return (p == get_block(p)->ptr); // Validate the pointer
        }
    }
    return (0); // Invalid pointer
}

// Function to free allocated memory
void free(void* ptr)
{
    if (valid_addr(ptr))
    {                               // Ensure the pointer is valid
        t_block b = get_block(ptr); // Get the block associated with the pointer
        b->free = 1;                // Mark the block as free

        if (b->prev && b->prev->free)
        {
            b = fusion(b->prev); // Merge with the previous block if it's free
        }

        if (b->next && b->next->free)
        {
            b = fusion(b); // Merge with the next block if it's free
        }
        else
        {
            if (b->prev)
            {
                b->prev->next = NULL;
            }
            else
            {
                base = NULL;
            }
            brk(b);
        }
    }
}