#include <stdint.h> // Include stdint for fixed-width types
#include <stdio.h>  // Include for logging
#include <string.h> // Include for memset function
#include <sys/mman.h>
#include <time.h> // Include for timestamp

#define LOG_FILE_PATH "/home/ignacio/iledesma/imem/memory_log.txt"

// Time variables used for logging
#define NANOSECONDS_IN_SECOND 1000000000

typedef enum
{
    MALLOC,
    CALLOC,
    REALLOC,
    FREE
} alloc_type;

typedef struct log_entry
{
    alloc_type type;
    void* ptr;
    size_t size;
    size_t total_allocated;
    size_t total_freed;
    struct log_entry* next;
    unsigned long op_id;
} t_log_entry;

void log_mem_operation(alloc_type type, void *ptr, size_t size);