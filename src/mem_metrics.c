#include "mem_metrics.h" // Include the header for memory metrics functionality.

// External variables from memory.c for accessing heap state and memory statistics.
extern void* base;                    // Pointer to the start of the heap.
extern int method;                    // Memory allocation method.
extern size_t total_allocated_memory; // Total memory allocated during program execution.
extern size_t total_freed_memory;     // Total memory freed during program execution.
extern int enable_unmapping;          // Flag to enable or disable unmapping of memory blocks.

/**
 * @brief Calculates fragmentation for all allocation methods.
 */
void calculate_fragmentation_all_methods(double* fragmentation_rates)
{
    if (base == NULL) // If the heap is empty, there's nothing to calculate.
    {
        printf("No blocks to calculate fragmentation.\n");
        for (int m = 0; m < ALLOC_METHODS; m++) // Set fragmentation rates to 0 for all methods.
        {
            fragmentation_rates[m] = 0.0; // Initialize fragmentation as 0%.
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
        if (m < 0 || m >= ALLOC_METHODS)     // Skip invalid methods (Free blocks created on purpose for example).
        {
            current_block = current_block->next;
            continue; // Move to the next block.
        }

        if (current_block->free) // If the block is free, update free memory stats.
        {
            total_free[m] += current_block->size;            // Add the block size to the free memory total.
            if (current_block->size > largest_free_block[m]) // Update the largest free block.
            {
                largest_free_block[m] = current_block->size;
            }
        }
        else // If the block is allocated, update allocated memory stats.
        {
            total_allocated[m] += current_block->size;
        }
        current_block = current_block->next; // Move to the next block.
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

        printf("%sFragmentation Report for %s%s:\n", YELLOW, method_name, RESET);
        printf("\tTotal memory: %zu bytes\n", total_memory);
        printf("\tTotal allocated memory: %zu bytes\n", total_allocated[m]);
        printf("\tTotal free memory: %zu bytes\n", total_free[m]);
        printf("\tLargest free block: %zu bytes\n", largest_free_block[m]);
        printf("\tExternal fragmentation: %.2f%%\n\n", external_fragmentation);
    }
}

void efficiency_test_all_methods(void)
{
    double fragmentation_rates[ALLOC_METHODS]; // Array to store fragmentation rates for each method.

    void* pointers[ALLOC_METHODS][NUM_ALLOCATIONS]; // Array to hold allocated blocks for all methods.
    size_t sizes[ALLOC_METHODS][NUM_ALLOCATIONS];   // Array to hold sizes for all methods.

    double allocation_times[ALLOC_METHODS] = {0};   // Array to store allocation times for each method.
    double deallocation_times[ALLOC_METHODS] = {0}; // Array to store deallocation times for each method.

    enable_unmapping = FALSE; // Disable unmapping during allocation.

    srand(time(NULL)); // Seed the random number generator.

    // Generate random sizes for all methods.
    for (int m = FIRST_FIT; m < ALLOC_METHODS; m++)
    {
        for (int i = 0; i < NUM_ALLOCATIONS; i++)
        {
            sizes[m][i] = rand() % ALLOCATION_SIZE_MAX + ALLOCATION_SIZE_MIN;
        }
    }

    // Interleave allocations for all methods and measure times.
    for (int m = FIRST_FIT; m < ALLOC_METHODS; m++)
    {
        set_method(m); // Set the allocation method.

        double start_time = get_time_in_milliseconds(); // Start timing allocation.

        for (int i = 0; i < NUM_ALLOCATIONS; i++)
        {
            pointers[m][i] = my_malloc(sizes[m][i]); // Allocate memory for this method.
            if (!pointers[m][i])
            {
                fprintf(stderr, "Allocation failed for method %d at iteration %d.\n", m, i);
                exit(1);
            }

            // Periodically free some blocks to simulate fragmentation.
            if (i % 5 == 0 && pointers[m][i])
            {
                my_free(pointers[m][i], FALSE); // Free every fifth block.
                pointers[m][i] = NULL;
            }
        }

        allocation_times[m] = get_time_in_milliseconds() - start_time; // End timing allocation.
    }

    // Calculate fragmentation after interleaved allocations and frees.
    calculate_fragmentation_all_methods(fragmentation_rates);

    // Deallocate all remaining memory blocks and measure times.
    for (int m = FIRST_FIT; m < ALLOC_METHODS; m++)
    {
        double start_time = get_time_in_milliseconds(); // Start timing deallocation.

        for (int i = 0; i < NUM_ALLOCATIONS; i++)
        {
            if (pointers[m][i])
            {
                my_free(pointers[m][i], TRUE); // Free the memory block.
                pointers[m][i] = NULL;         // Nullify the pointer to prevent double free.
            }
        }

        deallocation_times[m] = get_time_in_milliseconds() - start_time; // End timing deallocation.
    }

    // Print allocation and deallocation times for each method.
    for (int m = FIRST_FIT; m < ALLOC_METHODS; m++)
    {
        const char* method_name = (m == FIRST_FIT)   ? "First Fit"
                                  : (m == BEST_FIT)  ? "Best Fit"
                                  : (m == WORST_FIT) ? "Worst Fit"
                                                     : "Unknown";
        printf(BLUE "Efficiency Test Results for %s:\n" RESET, method_name);
        printf("\tAllocation time: %.6f [ms]\n", allocation_times[m]);
        printf("\tDeallocation time: %.6f [ms]\n\n", deallocation_times[m]);
    }

    enable_unmapping = TRUE; // Re-enable unmapping after the test.
}

double get_time_in_milliseconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Obtén el tiempo actual.
    return (ts.tv_sec * MILLISECONDS_IN_SECOND) +
           (ts.tv_nsec / NANOSECONDS_IN_MILLISECOND); // Convierte a milisegundos.
}

void clear_memory(void)
{
    enable_unmapping = TRUE; // Enable unmapping to release memory.

    if (base == NULL) // If the heap is empty, nothing to clear.
    {
        printf("No blocks to clear. The allocator is empty.\n");
        return;
    }

    t_block current_block = base; // Start at the base of the heap.
    while (current_block)         // Traverse the heap to unmap all blocks.
    {
        t_block next = current_block->next; // Save the next block pointer.
        size_t total_size = current_block->size + BLOCK_MIN_SIZE;
        if (munmap(current_block, total_size) == INVALID_ADDRESS) // Attempt to unmap the current block.
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
    printf("Cleared Heap and Logs\n");
}
