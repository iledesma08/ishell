#include "include/calloc.h"

// Function to allocate zero-initialized memory
void* calloc(size_t number, size_t size)
{
    size_t* new;
    size_t s, i;

    if (!number || !size)
    {
        return NULL;
    }

    new = malloc(number * size); // Allocate memory

    if (new)
    {
        s = align(number * size) << 2;
        for (i = 0; i < s; i++)
        {
            new[i] = 0; // Zero-initialize the memory
        }
    }
    return new;
}