#include "../include/realloc.h"
#include "../include/free.h"
#include "../include/malloc.h"

extern pthread_mutex_t memory_mutex; // Mutex to synchronize memory operations
unsigned long realloc_ctr = 0;

// Function to copy data from one block to another
void copy_block(t_block src, t_block dst)
{
    size_t *sdata, *ddata;
    size_t i;
    sdata = src->ptr;
    ddata = dst->ptr;
    for (i = 0; i * sizeof(size_t) < src->size && i * sizeof(size_t) < dst->size; i++)
        ddata[i] = sdata[i];
}

// Function to reallocate memory
void* realloc(void* ptr, size_t size)
{
    pthread_mutex_lock(&memory_mutex);

    if (ptr == NULL)
    {
        pthread_mutex_unlock(&memory_mutex);
        return malloc(size);
    }

    t_block b = get_block(ptr);
    if (!valid_addr(ptr) || b == NULL)
    {
        pthread_mutex_unlock(&memory_mutex);
        return NULL; // Invalid address, return NULL to prevent undefined behavior
    }

    if (b->size >= size)
    {
        pthread_mutex_unlock(&memory_mutex);
        return ptr;
    }

    void* new_ptr = malloc(size);
    if (new_ptr)
    {
        char *src = (char*)ptr, *dest = (char*)new_ptr;
        for (size_t i = 0; i < b->size; i++)
        {
            dest[i] = src[i];
        }
        free(ptr, FALSE); // Do not unmap during reallocation
    }

    pthread_mutex_unlock(&memory_mutex);
    return new_ptr;
}
