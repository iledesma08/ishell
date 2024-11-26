#include "../include/malloc.h"

extern void* base; // Base of the heap
extern int method; // Allocation method
extern pthread_mutex_t memory_mutex; // Mutex to synchronize memory operations

extern unsigned long first_fit_uses; // Counter for First Fit uses
extern unsigned long best_fit_uses; // Counter for Best Fit uses
extern unsigned long worst_fit_uses; // Counter for Worst Fit uses

extern unsigned long first_fit_alloc_ctr; // Counter for First Fit allocations
extern unsigned long best_fit_alloc_ctr; // Counter for Best Fit allocations
extern unsigned long worst_fit_alloc_ctr; // Counter for Worst Fit allocations
extern double first_fit_alloc_timestamp; // Total time spent in First Fit allocations
extern double best_fit_alloc_timestamp; // Total time spent in Best Fit allocations
extern double worst_fit_alloc_timestamp; // Total time spent in Worst Fit allocations

extern size_t total_allocated_memory; // Total allocated memory

unsigned long malloc_ctr = 0;

// Function to find a suitable memory block based on the allocation method
t_block find_block(t_block* last, size_t size)
{
    t_block b = base;

    if (method == FIRST_FIT)
    {
        while (b && !(b->free && b->size >= size))
        {
            *last = b;
            b = b->next;
        }
        if (b)
            first_fit_uses++;
        return b;
    }
    else if (method == BEST_FIT)
    {
        size_t diff = (size_t)-1; // Initialize to maximum size_t value
        t_block best = NULL;

        while (b)
        {
            if (b->free && b->size >= size)
            {
                if (b->size - size < diff)
                {
                    diff = b->size - size;
                    best = b;
                }
                if (diff == 0)
                { // Perfect fit
                    break;
                }
            }
            *last = b;
            b = b->next;
        }
        if (best)
            best_fit_uses++;
        return best;
    }
    else if (method == WORST_FIT)
    {
        size_t max_size = 0;
        t_block worst = NULL;

        while (b)
        {
            if (b->free && b->size >= size)
            {
                if (b->size > max_size)
                {
                    max_size = b->size;
                    worst = b;
                }
            }
            *last = b;
            b = b->next;
        }
        if (worst)
            worst_fit_uses++;
        return worst;
    }
    else
    {
        // Default to First Fit if method is unrecognized
        while (b && !(b->free && b->size >= size))
        {
            *last = b;
            b = b->next;
        }
        if (b)
            first_fit_uses++;
        return b;
    }
}

// Function to split a memory block into two
void split_block(t_block b, size_t s)
{
    // Ensure there is enough space for the new block
    if (b->size > s + BLOCK_SIZE)
    {
        t_block new_block = (t_block)(b->data + s); // Calculate address of the new block
        new_block->size = b->size - s - BLOCK_SIZE; // Update size of the new block
        new_block->next = b->next;                  // Link the new block to the next block
        new_block->prev = b;                        // Update the previous link of the new block
        new_block->free = TRUE;                        // Mark new block as free
        new_block->ptr = new_block->data;           // Set pointer to the data segment of the new block

        b->size = s;         // Update the size of the current block
        b->next = new_block; // Link current block to the new block
        if (new_block->next)
        {
            new_block->next->prev = new_block; // Update the previous link of the next block
        }
    }
}

// Function to extend the heap by creating a new block
t_block extend_heap(t_block last, size_t s)
{
    t_block b;
    b = mmap(0, s + BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (b == MAP_FAILED)
    {
        return NULL;
    }
    b->size = s;
    b->next = NULL;
    b->prev = last;
    b->ptr = b->data;
    b->free = 0;
    b->alloc_method = method;

    if (last)
        last->next = b;

    return b;
}

// Function to allocate memory
void* malloc(size_t size)
{
    pthread_mutex_lock(&memory_mutex);

    t_block b, last;
    size_t s;
    struct timespec start, end;

    // Start timing the allocation process
    clock_gettime(CLOCK_MONOTONIC, &start);

    s = align(size);

    if (base)
    {
        last = base;
        b = find_block(&last, s);
        if (b)
        {
            if ((b->size - s) >= (BLOCK_SIZE + BLOCK_THRESHOLD))
            {
                split_block(b, s);
            }
            b->free = 0;
        }
        else
        {
            b = extend_heap(last, s);
            if (!b)
            {
                pthread_mutex_unlock(&memory_mutex);
                return NULL; // Return NULL if heap extension failed
            }
        }
    }
    else
    {
        b = extend_heap(NULL, s);
        if (!b)
        {
            pthread_mutex_unlock(&memory_mutex);
            return NULL; // Return NULL if heap extension failed
        }
        base = b;
    }

    b->alloc_method = method;
    total_allocated_memory += b->size;
    void* ptr = b->data;
    add_log_entry(MALLOC, ptr, size);

    // End timing the allocation process
    clock_gettime(CLOCK_MONOTONIC, &end);
    double op_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / NANOSECONDS_IN_SECOND;

    // Update allocation time and count for the current method
    if (method == FIRST_FIT)
    {
        first_fit_alloc_timestamp += op_time;
        first_fit_alloc_ctr++;
        first_fit_uses++;
    }
    else if (method == BEST_FIT)
    {
        best_fit_alloc_timestamp += op_time;
        best_fit_alloc_ctr++;
        best_fit_uses++;
    }
    else if (method == WORST_FIT)
    {
        worst_fit_alloc_timestamp += op_time;
        worst_fit_alloc_ctr++;
        worst_fit_uses++;
    }

    pthread_mutex_unlock(&memory_mutex);
    return ptr;
}
