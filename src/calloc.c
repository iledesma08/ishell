#include "../include/calloc.h"

extern pthread_mutex_t memory_mutex; // Mutex to synchronize memory operations
unsigned long calloc_ctr = 0;

// Function to allocate zero-initialized memory
void* calloc(size_t number, size_t size)
{
    pthread_mutex_lock(&memory_mutex);

    // Calculate the total size to allocate
    size_t total_size = number * size;
    if (total_size == 0)
    {
        pthread_mutex_unlock(&memory_mutex);
        return NULL;
    }

    // Allocate the memory using malloc
    void* ptr = malloc(total_size);
    if (ptr == NULL)
    {
        pthread_mutex_unlock(&memory_mutex);
        return NULL; // Return NULL if memory allocation failed
    }

    // Zero-initialize the allocated memory
    char* char_ptr = (char*)ptr;
    for (size_t i = 0; i < total_size; i++)
    {
        char_ptr[i] = 0;
    }

    // Log the calloc operation
    log_mem_operation(CALLOC, ptr, total_size, &calloc_ctr);

    pthread_mutex_unlock(&memory_mutex);
    return ptr;
}
