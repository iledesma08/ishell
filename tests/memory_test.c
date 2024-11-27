#include "memory_test.h" // Include the header for memory metrics functionality.

void test_memory_usage(void)
{
    printf("\n%s====== Running MEMORY_USAGE Tests ======%s\n", BLUE, RESET);
    printf("%s\nTest: EMPTY USAGE%s\n", YELLOW, RESET);
    memory_usage(); // Test empty usage

    printf("\n%s--------------------------------%s\n\n", YELLOW, RESET);

    printf("%sTest: ALLOCATIONS AND FREES%s\n\n", YELLOW, RESET);

    void* block6 = my_malloc(64);
    void* block7 = my_malloc(128);
    printf("%sAllocating blocks at %p and %p%s\n", RED, block6, block7, RESET);
    memory_usage();

    printf("%s\nFreing 64 bytes block at %p%s\n", GREEN, block6, RESET);
    my_free(block6, TRUE); // Free without merging
    memory_usage();

    printf("%s\nFreing 128 bytes block at %p%s\n", GREEN, block7, RESET);
    my_free(block7, TRUE); // Free without merging
    memory_usage();
}