#include "../include/mem_metrics.h"

// Access external variables from memory.c
extern void* base;
extern size_t total_allocated_memory;
extern size_t total_freed_memory;
extern int enable_unmapping;

// Implement calculate_fragmentation_per_method()
void calculate_fragmentation_per_method(double* fragmentation_rates)
{
    if (base == NULL)
    {
        printf("No blocks to calculate fragmentation.\n");
        for (int m = 0; m < ALLOC_METHODS; m++)
        {
            fragmentation_rates[m] = 0.0;
        }
        return;
    }

    size_t total_free[ALLOC_METHODS] = {0};
    size_t largest_free_block[ALLOC_METHODS] = {0};
    size_t total_allocated[ALLOC_METHODS] = {0};

    t_block current_block = base;
    while (current_block)
    {
        int m = current_block->alloc_method; // Allocation method
        if (m < 0 || m >= ALLOC_METHODS)
        {
            printf("Invalid allocation method in block at %p\n", (void*)current_block);
            current_block = current_block->next;
            continue;
        }

        if (current_block->free)
        {
            total_free[m] += current_block->size;
            if (current_block->size > largest_free_block[m])
            {
                largest_free_block[m] = current_block->size;
            }
        }
        else
        {
            total_allocated[m] += current_block->size;
        }
        current_block = current_block->next;
    }

    for (int m = 0; m < ALLOC_METHODS; m++)
    {
        size_t total_memory = total_free[m] + total_allocated[m];
        double external_fragmentation = 0.0;

        if (total_free[m] > 0)
        {
            external_fragmentation =
                ((double)(total_free[m] - largest_free_block[m]) / (double)total_free[m]) * TO_PERCENTAGE_MULTIPLIER;
        }

        fragmentation_rates[m] = external_fragmentation;

        // Print the fragmentation report for each method
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

// Implement get_time_in_seconds()
double get_time_in_seconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / NANOSECONDS_IN_SECOND;
}

void efficiency_test(void)
{
    enable_unmapping = FALSE; // Disable unmapping during the test

    void** pointers = malloc(NUM_ALLOCATIONS * sizeof(void*));
    size_t* sizes = malloc(NUM_ALLOCATIONS * sizeof(size_t));
    if (!pointers || !sizes)
    {
        fprintf(stderr, "Memory allocation failed for test arrays.\n");
        exit(1);
    }

    // Generate random sizes
    srand(time(NULL));
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        sizes[i] = rand() % ALLOCATION_SIZE_MAX + ALLOCATION_SIZE_MIN;
    }

    // Measure allocation time
    double start_time = get_time_in_seconds();
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        pointers[i] = malloc(sizes[i]);
        if (!pointers[i])
        {
            fprintf(stderr, "Allocation failed at iteration %d\n", i);
            exit(1);
        }
    }
    double allocation_time = get_time_in_seconds() - start_time;

    // Measure deallocation time
    start_time = get_time_in_seconds();
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
    {
        // Free all pointers
        if (pointers[i])
        {
            free(pointers[i], TRUE); // Allow unmapping after free in normal cases
            pointers[i] = NULL;      // Prevent double free
        }
        // Free all sizes
        if (sizes[i])
        {
            free(sizes[i], TRUE); // Allow unmapping after free in normal cases
            sizes[i] = 0;         // Prevent double free
        }
    }
    double deallocation_time = get_time_in_seconds() - start_time;

    // Print results
    printf(BLUE "Efficiency Test Results:\n" RESET);
    printf("\tAllocation time: %.6f seconds\n", allocation_time);
    printf("\tDeallocation time: %.6f seconds\n", deallocation_time);

    // Calculate fragmentation after test
    double fragmentation_rates[ALLOC_METHODS];
    calculate_fragmentation_per_method(fragmentation_rates);

    enable_unmapping = TRUE; // Re-enable unmapping after the test
}

void clear_allocator(void)
{
    enable_unmapping = 1; // Ensure unmapping is enabled

    if (base == NULL)
    {
        printf("No blocks to clear. The allocator is empty.\n");
        return;
    }

    t_block current_block = base;
    while (current_block)
    {
        t_block next = current_block->next;
        size_t total_size = current_block->size + BLOCK_SIZE;
        if (munmap(current_block, total_size) == -1)
        {
            perror("munmap failed");
        }
        else
        {
            printf("Unmapped block at %p, size: %zu\n", (void*)current_block, total_size);
        }
        current_block = next;
    }

    // Reset global allocator state
    base = NULL;
    total_allocated_memory = 0;
    total_freed_memory = 0;

    // Clear logs
    clear_logs();
    printf("Allocator state cleared and logs cleared.\n");
}
