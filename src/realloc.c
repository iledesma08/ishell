#include "include/realloc.h"

// Function to copy data from one block to another
void copy_block(t_block src, t_block dst)
{
    size_t i;

    int *sdata = src->ptr; // Source data pointer
    int *ddata = dst->ptr; // Destination data pointer

    // Copy data from the source block to the destination block
    for (i = 0; (i * 4) < src->size && (i * 4) < dst->size; i++)
    {
        ddata[i] = sdata[i];
    }
}

// Function to reallocate memory
void* realloc(void* ptr, size_t size)
{
    size_t s;
    t_block b, new;
    void* newp;

    if (!ptr)
        return malloc(size); // If the pointer is NULL, act like malloc

    if (valid_addr(ptr))
    { // Ensure the pointer is valid
        s = align(size);
        b = get_block(ptr);

        if (b->size >= s)
        { // If the block is already large enough
            if (b->size >= (BLOCK_SIZE + 4))
            {
                split_block(b, s); // Resize the block
            }
        }
        else
        {
            if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
            {
                fusion(b); // Merge with the next block if possible
                if (b->size - s >= (BLOCK_SIZE + 4))
                {
                    split_block(b, s);
                }
            }
            else
            {
                newp = malloc(s); // Allocate a new block
                if (!newp)
                    return NULL;
                new = get_block(newp);
                copy_block(b, new); // Copy data to the new block
                free(ptr);          // Free the old block
                return newp;
            }
        }
        return ptr;
    }
    return NULL;
}