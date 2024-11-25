#include "include/calloc.h"

// Function to allocate zero-initialized memory
void* calloc(size_t number, size_t size)
{
    size_t* new;
    size_t s4, i;

    if (!number || !size)
    {
        return (NULL);
    }
    new = malloc(number * size);
    if (new)
    {
        s4 = align(number * size) << 2;
        for (i = 0; i < s4; i++)
            new[i] = 0;
    }
    return (new);
}