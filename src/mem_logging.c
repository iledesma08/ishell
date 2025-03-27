#include "mem_logging.h" // Include the header file for memory logging functionality.

extern size_t total_allocated_memory; // Tracks the total amount of memory allocated.
extern size_t total_freed_memory;     // Tracks the total amount of memory freed.

t_log_entry* log_head = NULL; // Pointer to the head of the linked list for log entries.

void log_mem_operation(alloc_op op, void* ptr, size_t size, unsigned long* op_ctr)
{
    // Allocate memory for a new log entry using mmap.
    t_log_entry* entry = mmap(NULL, sizeof(t_log_entry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (entry == MAP_FAILED) // Check if the memory allocation failed.
    {
        // Print an error message to stderr if the allocation failed.
        fprintf(stderr, "Failed to allocate memory for log entry.\n");
        return; // Exit the function without logging.
    }

    // Populate the log entry with details about the memory operation.
    entry->op = op;                                  // Type of memory operation (e.g., MALLOC, FREE).
    entry->ptr = ptr;                                // Pointer to the memory block involved in the operation.
    entry->size = size;                              // Size of the memory block.
    entry->total_allocated = total_allocated_memory; // Total allocated memory at the time of the operation.
    entry->total_freed = total_freed_memory;         // Total freed memory at the time of the operation.
    entry->next = log_head;                          // Link the new entry to the current head of the log list.
    entry->op_id = *op_ctr;                          // Assign a unique operation ID to the log entry.
    (*op_ctr)++;                                     // Increment the operation counter.
    log_head = entry;                                // Update the head of the log list to the new entry.
}

void clear_logs(void)
{
    t_log_entry* entry = log_head; // Start at the head of the log list.

    // Traverse the log list and unmap each entry.
    while (entry)
    {
        t_log_entry* next = entry->next;    // Save the pointer to the next log entry.
        munmap(entry, sizeof(t_log_entry)); // Unmap the current log entry from memory.
        entry = next;                       // Move to the next entry in the list.
    }

    log_head = NULL; // Reset the head of the log list to indicate it is empty.
}

void show_logs(void)
{
    // Print the memory operation log.
    printf(YELLOW "\nOPERATIONS LOG:\n\n" RESET);

    t_log_entry* entry = log_head; // Start at the head of the log list.
    if (entry == NULL)             // If the log is empty, print a message and return.
    {
        printf("\tNo memory operations logged.\n");
        return;
    }

    // Reverse the log list to print in chronological order.
    t_log_entry* reversed_log = NULL;
    while (entry)
    {
        t_log_entry* next = entry->next;
        entry->next = reversed_log;
        reversed_log = entry;
        entry = next;
    }

    entry = reversed_log; // Traverse the reversed list.
    while (entry)
    {
        // Print each log entry with appropriate formatting based on operation type.
        switch (entry->op)
        {
        case MALLOC:
            printf("%smalloc [%lu]%s of %zu bytes at %p\n", RED, entry->op_id, RESET, entry->size, entry->ptr);
            break;
        case CALLOC:
            printf("%scalloc [%lu]%s of %zu bytes at %p\n", RED, entry->op_id, RESET, entry->size, entry->ptr);
            break;
        case REALLOC:
            printf("%srealloc [%lu]%s to %zu bytes at %p\n", RED, entry->op_id, RESET, entry->size, entry->ptr);
            break;
        case FREE:
            printf("%sfree [%lu]%s of %zu bytes from %p\n", GREEN, entry->op_id, RESET, entry->size, entry->ptr);
            break;
        }
        entry = entry->next; // Move to the next log entry.
    }
}
