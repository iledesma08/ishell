#include "../include/memory.h" // Custom memory management header

extern t_log_entry* log_head; // Head of the log list

void* base = NULL;      // Pointer to the beginning of the heap
int method = FIRST_FIT; // Memory allocation method (0 = First Fit, 1 = Best Fit)
int enable_unmapping = FALSE; // Enable unmapping of freed memory blocks
typedef struct s_block* t_block;
pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for thread safety

// Counters for allocation methods
unsigned long first_fit_uses = 0;
unsigned long best_fit_uses = 0;
unsigned long worst_fit_uses = 0;

// Counters for allocation time and count
unsigned long first_fit_alloc_ctr = 0;
unsigned long best_fit_alloc_ctr = 0;
unsigned long worst_fit_alloc_ctr = 0;
double first_fit_alloc_timestamp = 0;
double best_fit_alloc_timestamp = 0;
double worst_fit_alloc_timestamp = 0;

// Memory amounts to be tracked
size_t total_allocated_memory = 0;
size_t total_freed_memory = 0;

void memory_manager_init()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&memory_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

void memory_manager_cleanup()
{
    pthread_mutex_destroy(&memory_mutex);
}

// Function to get the block associated with a pointer
t_block get_block(void* p)
{
    if (p == NULL)
    {
        return NULL;
    }
    return (t_block)((char*)p - offsetof(struct s_block, data));
}

void set_method(int m)
{
    if (m == FIRST_FIT || m == BEST_FIT || m == WORST_FIT)
    {
        method = m;
    }
    else
    {
        printf("Error: invalid method\n");
    }
}

void check_heap(void)
{
    if (base == NULL)
    {
        printf("Heap is empty.\n");
        return;
    }

    t_block current = base;
    printf(YELLOW "Heap check\n" RESET);
    while (current != NULL)
    {
        if (current->free && current->next && current->next->free)
        {
            printf("Warning: Free blocks at %p and %p are adjacent but not fused.\n", (void*)current,
                   (void*)current->next);
        }
        if (current->size <= 0)
        {
            printf("Warning: Block at %p has invalid size %zu.\n", (void*)current, current->size);
        }
        printf("Block at %p\n", (void*)current);
        printf("  Size: %zu\n", current->size);
        printf("  Free: %d\n", current->free);
        printf("  Next block: %p\n", (void*)(current->next));
        printf("  Previous block: %p\n", (void*)(current->prev));
        printf("  Data address: %p\n", current->ptr);
        current = current->next;
    }
}

void memory_usage(void)
{
    size_t current_allocated = total_allocated_memory - total_freed_memory;
    printf(YELLOW "Memory Usage Report:\n" RESET);
    printf("  Total allocated memory (since start): %zu bytes\n", total_allocated_memory);
    printf("  Total freed memory (since start): %zu bytes\n", total_freed_memory);
    printf("  Currently allocated memory: %zu bytes\n", current_allocated);

    // Display the log entries
    printf(BLUE "\nMemory Operation Log:\n" RESET);
    t_log_entry* entry = log_head;

    // Reverse the log list to print in chronological order
    t_log_entry* reversed_log = NULL;
    while (entry)
    {
        t_log_entry* next = entry->next;
        entry->next = reversed_log;
        reversed_log = entry;
        entry = next;
    }

    entry = reversed_log;
    while (entry)
    {
        switch (entry->type)
        {
        case MALLOC:
            printf(RED "malloc" RESET " of %zu bytes at %p\n", entry->size, entry->ptr);
            break;
        case CALLOC:
            printf(RED "calloc" RESET " of %zu bytes at %p\n", entry->size, entry->ptr);
            break;
        case REALLOC:
            printf(RED "realloc" RESET " to %zu bytes at %p\n", entry->size, entry->ptr);
            break;
        case FREE:
            printf(GREEN "free" RESET " of %zu bytes from %p\n", entry->size, entry->ptr);
            break;
        }
        entry = entry->next;
    }
}
