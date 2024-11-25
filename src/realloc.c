#include "../include/realloc.h"

// Function to copy data from one block to another
void copy_block(t_block src, t_block dst)
{
    int *sdata, *ddata;
    size_t i;
    sdata = src->ptr;
    ddata = dst->ptr;

    for (i = 0; (i * 4) < src->size && (i * 4) < dst->size; i++)
        ddata[i] = sdata[i];
}

// Function to reallocate memory
void* realloc(void* ptr, size_t size)
{
    size_t s;
    t_block b, new;
    void* newp;

    if (!ptr)
        return (malloc(size));

    if (valid_addr(ptr))
    {
        s = align(size);
        b = get_block(ptr);

        if (b->size >= s)
        {
            if (b->size - s >= (BLOCK_SIZE + 4))
                split_block(b, s);
        }
        else
        {
            if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
            {
                fusion(b);
                if (b->size - s >= (BLOCK_SIZE + 4))
                    split_block(b, s);
            }
            else
            {
                newp = malloc(s);
                if (!newp)
                    return (NULL);
                new = get_block(newp);
                copy_block(b, new);
                free(ptr);
                return (newp);
            }
        }
        return (ptr);
    }
    return (NULL);
}