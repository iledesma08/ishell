#include "../include/free.h"

extern void* base; // Base of the heap

// Function to merge adjacent free memory blocks
t_block fusion(t_block b)
{
    // If next block exists and is free, merge it with the current block
    if (b->next && b->next->free)
    {
        b->size += BLOCK_SIZE + b->next->size; // Combine sizes of current and next block
        b->next = b->next->next;               // Update next pointer to skip merged block

        // Update the previous link of the next block
        if (b->next)
        {
            b->next->prev = b;
        }
    }
    return b; // Return the merged block
}

// Function to check if a pointer is valid and part of the heap
int valid_addr(void* p)
{
    if (base)
    {
        // Ensure the pointer is within the heap bounds
        if (p > base && p < sbrk(0))
        {
            t_block b = get_block(p); // Retrieve block from pointer
            // Check if the pointer matches the start of the data segment
            return b && (p == b->ptr);
        }
    }
    return 0; // Invalid address
}

// Function to free allocated memory
void free(void* ptr)
{
    // Check if the pointer is valid before proceeding
    if (valid_addr(ptr))
    {
        t_block b = get_block(ptr); // Retrieve the block from the pointer
        b->free = 1;                // Mark the block as free

        // Merge with the next block if it is also free
        if (b->next && b->next->free)
        {
            fusion(b);
        }
        // Merge with the previous block if it is also free
        if (b->prev && b->prev->free)
        {
            fusion(b->prev);
        }
    }
}