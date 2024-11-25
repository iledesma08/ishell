#include "../include/realloc.h"

// Function to copy data from one block to another
void copy_block(t_block src, t_block dst)
{
    char* sdata = src->ptr;                                           // Pointer to source data
    char* ddata = dst->ptr;                                           // Pointer to destination data
    size_t copy_size = src->size < dst->size ? src->size : dst->size; // Determine the smaller size to copy

    // Use memcpy to copy data for better efficiency
    memcpy(ddata, sdata, copy_size);
}

// Function to reallocate memory
void* realloc(void* ptr, size_t size)
{
    if (size == 0)
    { // If new size is 0, free the memory and return NULL
        free(ptr);
        return NULL;
    }

    if (!ptr)
    { // If pointer is NULL, behave like malloc
        return malloc(size);
    }

    if (valid_addr(ptr))
    {                               // If the pointer is valid
        size_t s = align(size);     // Align the requested size
        t_block b = get_block(ptr); // Get the block from the pointer

        if (b->size >= s)
        { // If the current block is large enough
            if (b->size - s >= (BLOCK_SIZE + 4))
            {
                split_block(b, s); // Split the block if there's enough extra space
            }
            return ptr; // Return the original pointer
        }
        else
        { // If the current block is not large enough
            // Attempt to merge with the next block if possible
            if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
            {
                fusion(b); // Merge with the next block
                if (b->size - s >= (BLOCK_SIZE + 4))
                {
                    split_block(b, s); // Split if there's extra space
                }
                return ptr; // Return the original pointer
            }
            else
            { // Allocate a new block and copy data
                void* newp = malloc(s);
                if (!newp)
                {
                    return NULL; // Return NULL if allocation fails
                }
                t_block new_block = get_block(newp); // Get the block for the new pointer
                copy_block(b, new_block);            // Copy data to the new block
                free(ptr);                           // Free the old block
                return newp;                         // Return the new pointer
            }
        }
    }
    return NULL; // Return NULL if the pointer is invalid
}