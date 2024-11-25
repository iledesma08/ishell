#include "include/malloc.h"

extern void* base; // Base of the heap
extern int method; // Allocation method: FIRST_FIT or BEST_FIT

// Function to find a suitable memory block based on the allocation method
t_block find_block(t_block* last, size_t size)
{
    t_block b = base;

    if (method == FIRST_FIT)
    {
        // Iterate through blocks until a suitable free block is found
        while (b && !(b->free && b->size >= size))
        {
            *last = b;
            b = b->next;
        }
        return b; // Return the found block or NULL if not found
    }
    else
    {                          // Best fit strategy
        size_t dif = SIZE_MAX; // Start with the maximum difference
        t_block best = NULL;

        // Iterate through blocks to find the best fit
        while (b)
        {
            if (b->free && b->size >= size && (b->size - size) < dif)
            {
                dif = b->size - size;
                best = b; // Update best block if a closer fit is found
            }
            *last = b;
            b = b->next;
        }
        return best; // Return the best fit block or NULL if not found
    }
}

// Function to split a memory block into two
t_block split_block(t_block b, size_t s)
{
    // Ensure there is enough space for the new block
    if (b->size > s + BLOCK_SIZE)
    {
        t_block new_block = (t_block)(b->data + s); // Calculate address of the new block
        new_block->size = b->size - s - BLOCK_SIZE; // Update size of the new block
        new_block->next = b->next;                  // Link the new block to the next block
        new_block->free = 1;                        // Mark new block as free

        b->size = s;         // Update the size of the current block
        b->next = new_block; // Link current block to the new block
        new_block->prev = b; // Update the previous link of the new block
    }
    return b;
}

// Function to extend the heap by creating a new block
t_block extend_heap(t_block last, size_t s)
{
    // Allocate new memory using mmap for heap extension
    t_block b = mmap(0, s + BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (b == MAP_FAILED)
    {
        return NULL; // If mmap fails, return NULL
    }

    // Set up new block details
    b->size = s;
    b->next = NULL;
    b->prev = last;
    b->ptr = b->data; // Set the pointer to the data segment
    b->free = 0;      // Mark block as allocated

    // Update the link from the last block if it exists
    if (last)
    {
        last->next = b;
    }
    return b;
}

// Function to allocate memory
void* malloc(size_t size)
{
    // Prevent integer overflow in size alignment
    if (size == 0 || size > SIZE_MAX - BLOCK_SIZE)
    {
        return NULL; // Return NULL if size is invalid
    }

    size_t s = align(size); // Align the requested size
    t_block b, last;

    if (base)
    { // If the heap is initialized
        last = base;
        b = find_block(&last, s); // Find a suitable block
        if (b)
        {
            // Split block if there is enough extra space
            if ((b->size - s) >= (BLOCK_SIZE + 4))
            {
                split_block(b, s);
            }
            b->free = 0; // Mark block as allocated
        }
        else
        { // No suitable block found, extend the heap
            b = extend_heap(last, s);
            if (!b)
            {
                return NULL; // Return NULL if heap extension fails
            }
        }
    }
    else
    {                             // If heap is not initialized
        b = extend_heap(NULL, s); // Extend heap for the first time
        if (!b)
        {
            return NULL; // Return NULL if heap extension fails
        }
        base = b; // Set the base to the newly allocated block
    }
    return b->data; // Return the data pointer of the allocated block
}
