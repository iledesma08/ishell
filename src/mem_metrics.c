#include "../include/mem_metrics.h" // Include the header for memory metrics functionality.

// External variables from memory.c for accessing heap state and memory statistics.
extern void* base;                   // Pointer to the start of the heap.
extern size_t total_allocated_memory; // Total memory allocated during program execution.
extern size_t total_freed_memory;    // Total memory freed during program execution.
extern int enable_unmapping;         // Flag to enable or disable unmapping of memory blocks.

/* Calculates fragmentation for each memory allocation method. */
void calculate_fragmentation_per_method(double* fragmentation_rates)
{
    if (base == NULL) // If the heap is empty, there's nothing to calculate.
    {
        printf("No blocks to calculate fragmentation.\n");
        for (int m = 0; m < ALLOC_METHODS; m++) // Set fragmentation rates to 0 for all methods.
        {
            fragmentation_rates[m] = 0.0;
        }
        return;
    }

    size_t total_free[ALLOC_METHODS] = {0};         // Total free memory for each method.
    size_t largest_free_block[ALLOC_METHODS] = {0}; // Largest free block for each method.
    size_t total_allocated[ALLOC_METHODS] = {0};    // Total allocated memory for each method.

    t_block current_block = base; // Start from the base of the heap.
    while (current_block)
    {
        int m = current_block->alloc_method; // Get the allocation method for the block.
        if (m < 0 || m >= ALLOC_METHODS) // Skip invalid methods.
        {
            printf("Invalid allocation method in block at %p\n", (void*)current_block);
            current_block = current_block->next;
            continue;
        }

        if (current_block->free) // If the block is free, update free memory stats.
        {
            total_free[m] += current_block->size;
            if (current_block->size > largest_free_block[m])
            {
                largest_free_block[m] = current_block->size;
            }
        }
        else // If the block is allocated, update allocated memory stats.
        {
            total_allocated[m] += current_block->size;
        }
        current_block = current_block->next;
    }

    for (int m = 0; m < ALLOC_METHODS; m++)
    {
        size_t total_memory = total_free[m] + total_allocated[m]; // Total memory for the method.
        double external_fragmentation = 0.0;

        if (total_free[m] > 0) // Calculate fragmentation if there's free memory.
        {
            external_fragmentation =
                ((double)(total_free[m] - largest_free_block[m]) / (double)total_free[m]) * TO_PERCENTAGE_MULTIPLIER;
        }

        fragmentation_rates[m] = external_fragmentation; // Store the calculated fragmentation rate.

        // Print fragmentation details for the current allocation method.
        const char* method_name = (m == FIRST_FIT)   ? "First Fit"
                                  : (m == BEST_FIT)  ? "Best Fit"
                                  : (m == WORST_FIT) ? "Worst Fit"
                                                     : "Unknown";

        printf("Fragmentation Report for %s:\n", method_name);
        printf("  Total memory: %zu bytes\n", total_memory);
        printf("  Total allocated memory: %zu bytes\n", total_allocated[m]);
        printf("  Total free memory: %zu bytes\n", total_free[m]);
        printf("  Largest free block: %zu bytes\n", largest_free_block[m]);
        printf("  External fragmentation: %.2f%%\n", external_fragmentation);
    }
}

/* Returns the current time in seconds, combining seconds and nanoseconds. */
double get_time_in_seconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Get the current time.
    return ts.tv_sec + ts.tv_nsec / NANOSECONDS_IN_SECOND; // Convert to seconds.
}

/* Tests the efficiency of the allocator by measuring allocation and deallocation times. */
void efficiency_test(void)
{
    enable_unmapping = FALSE; // Disable unmapping during the test to avoid overhead.

    // Allocate arrays for pointers and sizes.
    void** pointers = malloc(NUM_ALLOCATIONS * sizeof(void*));
    size_t* sizes = malloc(NUM_ALLOCATIONS * sizeof(size_t));
    if (!pointers || !sizes) // Check if memory allocation for test arrays failed.
    {
        fprintf(stderr, "Memory allocation failed for test arrays.\n");
        exit(1);
    }

    // Generate random sizes for allocations.
    srand(time(NULL));
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        sizes[i] = rand() % ALLOCATION_SIZE_MAX + ALLOCATION_SIZE_MIN;
    }

    // Measure the time to allocate memory.
    double start_time = get_time_in_seconds();
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        pointers[i] = malloc(sizes[i]);
        if (!pointers[i]) // Check if allocation failed for any pointer.
        {
            fprintf(stderr, "Allocation failed at iteration %d\n", i);
            exit(1);
        }
    }
    double allocation_time = get_time_in_seconds() - start_time;

    // Measure the time to deallocate memory.
    start_time = get_time_in_seconds();
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        if (pointers[i])
        {
            free(pointers[i], TRUE); // Free each pointer, allowing unmapping.
            pointers[i] = NULL;      // Nullify the pointer to prevent double free.
        }
        if (sizes[i]) // Reset size values to avoid reuse.
        {
            free(sizes[i], TRUE);
            sizes[i] = 0;
        }
    }
    double deallocation_time = get_time_in_seconds() - start_time;

    // Print the results of the efficiency test.
    printf(BLUE "Efficiency Test Results:\n" RESET);
    printf("\tAllocation time: %.6f seconds\n", allocation_time);
    printf("\tDeallocation time: %.6f seconds\n", deallocation_time);

    // Calculate and print fragmentation after the test.
    double fragmentation_rates[ALLOC_METHODS];
    calculate_fragmentation_per_method(fragmentation_rates);

    enable_unmapping = TRUE; // Re-enable unmapping after the test.
}

/* Clears all blocks and resets the allocator to its initial state. */
void clear_allocator(void)
{
    enable_unmapping = 1; // Enable unmapping to release memory.

    if (base == NULL) // If the heap is empty, nothing to clear.
    {
        printf("No blocks to clear. The allocator is empty.\n");
        return;
    }

    t_block current_block = base; // Start at the base of the heap.
    while (current_block) // Traverse the heap to unmap all blocks.
    {
        t_block next = current_block->next; // Save the next block pointer.
        size_t total_size = current_block->size + BLOCK_SIZE;
        if (munmap(current_block, total_size) == -1) // Attempt to unmap the current block.
        {
            perror("munmap failed");
        }
        else
        {
            printf("Unmapped block at %p, size: %zu\n", (void*)current_block, total_size);
        }
        current_block = next; // Move to the next block.
    }

    // Reset global variables to their initial state.
    base = NULL;
    total_allocated_memory = 0;
    total_freed_memory = 0;

    clear_logs(); // Clear the memory operation logs.
    printf("Allocator state cleared and logs cleared.\n");
}
