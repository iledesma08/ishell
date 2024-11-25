#include "include/malloc.h"

extern base;
extern method;

// Function to find a suitable memory block based on the allocation method
t_block find_block(t_block* last, size_t size)
{
    t_block b = base;

    if (method == FIRST_FIT)
    { // First Fit strategy
        while (b && !(b->free && b->size >= size))
        {                // Look for the first block large enough and free
            *last = b;   // Update the last block traversed
            b = b->next; // Move to the next block
        }
        return (b); // Return the found block or NULL
    }
    else
    {                          // Best Fit strategy
        size_t dif = PAGESIZE; // Difference between the requested size and the block size
        t_block best = NULL;   // Pointer to the best-fit block

        while (b)
        {
            if (b->free)
            { // If the block is free
                if (b->size == size)
                { // Perfect match
                    return b;
                }
                if (b->size > size && (b->size - size) < dif)
                { // Smaller difference is better
                    dif = b->size - size;
                    best = b;
                }
            }
            *last = b;   // Update the last traversed block
            b = b->next; // Move to the next block
        }
        return best; // Return the best-fit block or NULL
    }
}

// Function to split a memory block into two
void split_block(t_block b, size_t s)
{
    if (b->size <= s + BLOCK_SIZE)
    { // Check if the block can be split
        return;
    }

    t_block new;                          // Create a new block for the leftover memory
    new = (t_block)(b->data + s);         // Assign the new block after the allocated space
    new->size = b->size - s - BLOCK_SIZE; // Adjust size for the new block
    new->next = b->next;                  // Link the new block to the next block
    new->free = 1;                        // Mark the new block as free
    new->ptr = new->data;
    new->prev = b;
    b->next = new; // Update the next link of the original block
    b->size = s;   // Resize the original block
    if (new->next)
    {
        new->next->prev = new;
    }
}

// Function to extend the heap by creating a new block
t_block extend_heap(t_block last, size_t s)
{
    t_block old_break, new_break;
    old_break = sbrk(0);
    new_break = sbrk(BLOCK_SIZE + s);
    if (new_break == (void *)-1)
    {
        return NULL;
    }
    old_break->size = s;
    old_break->free = 0;
    old_break->next = NULL;
    old_break->prev = NULL;
    old_break->ptr = old_break->data;
    if (last)
    {
        last->next = old_break;
    }
    return (old_break);
}

// Function to allocate memory
void* malloc(size_t size)
{
    t_block b, last;
    size_t s = align(size); // Align the requested size

    if (base)
    { // If the heap exists
        last = base;
        b = find_block(&last, s); // Find a suitable block
        if (b)
        {
            if ((b->size - s) >= (BLOCK_SIZE + 4))
            {
                split_block(b, s); // Split the block if there's excess space
            }
            b->free = 0; // Mark the block as allocated
        }
        else
        {
            b = extend_heap(last, s); // Extend the heap if no block is found
            if (!b)
                return NULL;
        }
    }
    else
    { // Initialize the heap
        b = extend_heap(NULL, s);
        if (!b)
            return NULL;
        base = b; // Set the base pointer
    }
    return b->data; // Return a pointer to the allocated space
}