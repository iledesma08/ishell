#include "../include/realloc.h" // Include header file for realloc-related functions and definitions.

extern pthread_mutex_t memory_mutex; // Mutex for thread-safe memory operations.
unsigned long realloc_ctr = 0;       // Counter to track the number of realloc operations performed.

void copy_block(t_block src, t_block dst)
{
    size_t *sdata, *ddata; // Pointers for source and destination data.
    size_t i;              // Loop index for copying data.

    sdata = src->ptr; // Get the pointer to the source data.
    ddata = dst->ptr; // Get the pointer to the destination data.

    // Copy data byte by byte while ensuring it fits within both blocks' sizes.
    for (i = 0; i * sizeof(size_t) < src->size && i * sizeof(size_t) < dst->size; i++)
        ddata[i] = sdata[i];
}

void* my_realloc(void* ptr, size_t size)
{
    pthread_mutex_lock(&memory_mutex); // Lock the mutex for thread-safe operation.

    if (ptr == NULL) // If the pointer is NULL, realloc behaves like malloc.
    {
        pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before returning.
        return my_malloc(size);                 // Allocate a new block of the requested size.
    }

    t_block b = get_block(ptr); // Retrieve the block metadata for the pointer.

    // Check if the pointer is valid and belongs to the heap.
    if (!valid_addr(ptr) || b == NULL)
    {
        pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before returning.
        return NULL;                         // Return NULL for invalid address or metadata issues.
    }

    if (b->size >= size) // If the current block size is already sufficient.
    {
        pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before returning.
        return ptr;                          // Return the original pointer.
    }

    // Allocate a new block with the requested size.
    void* new_ptr = my_malloc(size);
    if (new_ptr) // Check if the allocation was successful.
    {
        char *src = (char*)ptr, *dest = (char*)new_ptr; // Cast pointers for byte-wise copying.

        // Copy data from the original block to the new block.
        for (size_t i = 0; i < b->size; i++)
        {
            dest[i] = src[i];
        }

        my_free(ptr, FALSE); // Free the old block without unmapping it.

        // Log the realloc operation for debugging and tracking.
        log_mem_operation(REALLOC, new_ptr, size, &realloc_ctr);
    }

    pthread_mutex_unlock(&memory_mutex); // Unlock the mutex after completing the operation.
    return new_ptr;                      // Return the pointer to the newly allocated memory.
}
