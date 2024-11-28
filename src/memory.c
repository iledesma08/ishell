#include "memory.h" // Includes the custom memory management library header.

extern t_log_entry* log_head; // External variable pointing to the head of the memory operation log list.

void* base = NULL;                                        // Pointer to the beginning of the memory heap.
int method = FIRST_FIT;                                   // Memory allocation method; default is First Fit (0).
int enable_unmapping = FALSE;                             // Flag to enable or disable unmapping of freed memory blocks.
typedef struct s_block* t_block;                          // Alias for a pointer to the `s_block` structure.
pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for synchronizing memory operations across threads.

// Memory usage statistics.
size_t total_allocated_memory = 0; // Tracks the total allocated memory.
size_t total_freed_memory = 0;     // Tracks the total freed memory.

void memory_manager_init()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);                             // Initialize mutex attributes.
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); // Set mutex to recursive type.
    pthread_mutex_init(&memory_mutex, &attr);                  // Initialize the mutex with the configured attributes.
    pthread_mutexattr_destroy(&attr); // Destroy the mutex attributes as they are no longer needed.
}

void memory_manager_cleanup()
{
    pthread_mutex_destroy(&memory_mutex); // Destroy the mutex to release resources.
}

t_block get_block(void* p)
{
    if (p == NULL) // If the pointer is null, return NULL.
    {
        return NULL;
    }
    // Calculate the start of the block structure using the offset of the `data` field.
    return (t_block)((char*)p - offsetof(struct s_block, data));
}

void set_method(int m)
{
    if (m == FIRST_FIT || m == BEST_FIT || m == WORST_FIT) // Check if the method is valid.
    {
        method = m; // Set the allocation method.
    }
    else
    {
        printf("Error: invalid method\n"); // Print an error if the method is invalid.
    }
}

void check_heap(void)
{
    if (base == NULL) // If the heap is empty, print a message and return.
    {
        printf("Heap is empty.\n");
        return;
    }

    t_block current = base; // Start at the base of the heap.
    while (current != NULL) // Iterate through all blocks in the heap.
    {
        // Check for adjacent free blocks that are not fused together.
        if (current->free && current->next && current->next->free)
        {
            printf("%sWarning:%s Free blocks at %p and %p are adjacent but not fused.\n", RED, RESET, (void*)current,
                   (void*)current->next);
        }
        // Check if the block size is invalid.
        if (current->size <= 0)
        {
            printf("%sWarning:%sBlock at %p has invalid size %zu.\n", RED, RESET, (void*)current, current->size);
        }
        // Print details of the current block.
        printf("%s--------------------------------%s\n", YELLOW, RESET);
        printf("%sBlock at %p%s\n", GRAY, (void*)current, RESET);
        printf("\tSize: %zu\n", current->size);
        printf("\tFree: %d\n", current->free);
        printf("\tNext block: %p\n", (void*)(current->next));
        printf("\tPrevious block: %p\n", (void*)(current->prev));
        printf("\tData address: %p\n", current->ptr);
        current = current->next; // Move to the next block.
    }
}

void memory_usage(void)
{
    printf(YELLOW "\nMemory Usage:\n" RESET);
    printf("\tTotal allocated memory (since start): %zu bytes\n", total_allocated_memory);
    printf("\tTotal freed memory (since start): %zu bytes\n", total_freed_memory);
    size_t current_allocated = total_allocated_memory - total_freed_memory; // Calculate currently allocated memory.
    printf("\tCurrently allocated memory: %zu bytes\n", current_allocated);
}
