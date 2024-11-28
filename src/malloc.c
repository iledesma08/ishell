#include "malloc.h" // Include the header file for memory allocation functions.

extern void* base;                   // Global pointer to the beginning of the heap.
extern int method;                   // Global variable indicating the memory allocation strategy.
extern pthread_mutex_t memory_mutex; // Mutex for thread-safe memory operations.

extern size_t total_allocated_memory; // Tracks the total memory allocated by the custom allocator.

unsigned long malloc_ctr = 0; // Counter for the number of malloc operations performed.

t_block find_block(t_block* last, size_t size)
{
    t_block b = base; // Start searching from the base of the heap.

    if (method == FIRST_FIT) // First Fit strategy: find the first block that fits.
    {
        while (b && !(b->free && b->size >= size)) // Traverse until a suitable block is found.
        {
            *last = b;   // Keep track of the last block traversed.
            b = b->next; // Move to the next block.
        }
        return b; // Return the found block or NULL if none is suitable.
    }
    else if (method == BEST_FIT) // Best Fit strategy: find the smallest block that fits.
    {
        size_t diff = (size_t)-1; // Initialize difference to the largest possible value.
        t_block best = NULL;      // Pointer to the best-fit block.

        while (b)
        {
            if (b->free && b->size >= size) // Check if the block is free and fits the requested size.
            {
                if (b->size - size < diff) // Update best fit if the current block is a closer match.
                {
                    diff = b->size - size;
                    best = b;
                }
                if (diff == 0) // If the block perfectly fits, stop searching.
                {
                    break;
                }
            }
            *last = b;
            b = b->next;
        }
        return best; // Return the best-fit block or NULL.
    }
    else if (method == WORST_FIT) // Worst Fit strategy: find the largest block that fits.
    {
        size_t max_size = 0;  // Initialize the maximum size to 0.
        t_block worst = NULL; // Pointer to the worst-fit block.

        while (b)
        {
            if (b->free && b->size >= size) // Check if the block is free and fits the requested size.
            {
                if (b->size > max_size) // Update worst fit if the current block is larger.
                {
                    max_size = b->size;
                    worst = b;
                }
            }
            *last = b;
            b = b->next;
        }
        return worst; // Return the worst-fit block or NULL.
    }
    else
    {
        // Default to First Fit if the method is invalid or unrecognized.
        while (b && !(b->free && b->size >= size))
        {
            *last = b;
            b = b->next;
        }
        return b; // Return the found block or NULL.
    }
}

void split_block(t_block b, size_t s)
{
    if (b->size > s + BLOCK_MIN_SIZE) // Ensure there is enough space to split.
    {
        t_block new_block = (t_block)(b->data + s);     // Calculate the address of the new block.
        new_block->size = b->size - s - BLOCK_MIN_SIZE; // Adjust the size of the new block.
        new_block->next = b->next;                      // Link the new block to the next block.
        new_block->prev = b;                            // Set the previous block of the new block.
        new_block->free = TRUE;                         // Mark the new block as free.
        new_block->ptr = new_block->data;               // Initialize the data pointer for the new block.

        b->size = s;         // Update the size of the current block.
        b->next = new_block; // Link the current block to the new block.
        if (new_block->next)
        {
            new_block->next->prev = new_block; // Update the previous link of the next block.
        }
    }
}

t_block extend_heap(t_block last, size_t s)
{
    t_block b =
        mmap(0, s + BLOCK_MIN_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); // Allocate memory.

    if (b == MAP_FAILED) // Check if memory allocation failed.
    {
        return NULL;
    }
    b->size = s;              // Set the size of the new block.
    b->next = NULL;           // New block is at the end, so no next block.
    b->prev = last;           // Link the new block to the previous block.
    b->ptr = b->data;         // Initialize the data pointer.
    b->free = FALSE;          // Mark the block as allocated.
    b->alloc_method = method; // Record the allocation method.

    if (last)
        last->next = b; // Link the previous block to the new block.

    return b; // Return the new block.
}

void* my_malloc(size_t size)
{
    pthread_mutex_lock(&memory_mutex); // Lock the mutex for thread safety.

    t_block b, last;       // Variables for the current and last blocks.
    size_t s;              // Aligned size of the requested memory.
    struct timespec start; // Variables for timing the operation.

    clock_gettime(CLOCK_MONOTONIC, &start); // Start timing.

    s = align(size); // Align the requested size.

    if (base) // Check if the heap has been initialized.
    {
        last = base;
        b = find_block(&last, s); // Try to find a suitable block.
        if (b)
        {
            if ((b->size - s) >= (BLOCK_MIN_SIZE + ALIGNMENT)) // Check if the block can be split.
            {
                split_block(b, s); // Split the block if necessary.
            }
            b->free = FALSE; // Mark the block as allocated.
        }
        else
        {
            b = extend_heap(last, s); // Extend the heap if no suitable block was found.
            if (!b)
            {
                pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before returning.
                return NULL;                         // Return NULL if the heap extension failed.
            }
        }
    }
    else
    {
        b = extend_heap(NULL, s); // Initialize the heap if it is empty.
        if (!b)
        {
            pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before returning.
            return NULL;                         // Return NULL if the heap extension failed.
        }
        base = b; // Set the base of the heap.
    }

    b->alloc_method = method;                          // Record the allocation method for the block.
    total_allocated_memory += b->size;                 // Update the total allocated memory.
    void* ptr = b->data;                               // Get the pointer to the allocated data.
    log_mem_operation(MALLOC, ptr, size, &malloc_ctr); // Log the memory allocation operation.

    pthread_mutex_unlock(&memory_mutex); // Unlock the mutex after completing the allocation.
    return ptr;                          // Return the pointer to the allocated memory.
}
