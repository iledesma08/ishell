#include "calloc.h"

extern pthread_mutex_t memory_mutex; // Mutex for thread-safe memory operations.
unsigned long calloc_ctr = 0;        // Counter to track the number of calloc operations performed.

void* my_calloc(size_t number, size_t size)
{
    pthread_mutex_lock(&memory_mutex); // Lock the mutex to ensure thread safety.

    // Calculate the total size of memory required.
    size_t total_size = number * size;
    if (total_size == 0) // If the calculated size is zero, return NULL.
    {
        pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before returning.
        return NULL;
    }

    pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before my_malloc starts.
    // Allocate the memory using malloc, leveraging the existing allocation logic.
    void* ptr = my_malloc(total_size);
    if (ptr == NULL) // Check if the allocation was successful.
    {
        pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before returning.
        return NULL;                         // Return NULL if allocation failed.
    }

    // Initialize the allocated memory to zero byte by byte.
    char* char_ptr = (char*)ptr; // Cast the pointer to a character pointer for byte-wise access.
    for (size_t i = 0; i < total_size; i++)
    {
        char_ptr[i] = 0; // Set each byte to zero.
    }

    // Log the calloc operation for tracking and debugging purposes.
    log_mem_operation(CALLOC, ptr, total_size, &calloc_ctr);

    pthread_mutex_unlock(&memory_mutex); // Unlock the mutex after completing the operation.
    return ptr;                          // Return the pointer to the zero-initialized memory.
}
