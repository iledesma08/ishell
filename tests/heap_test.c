#include "heap_test.h" // Include the header for heap test functionality.

void test_check_heap(void)
{
    printf("\n%s====== Running CHECK_HEAP Tests ======%s\n", BLUE, RESET);

    printf("%s\nTest: NON-EMPTY HEAP%s\n", YELLOW, RESET);
    void* block1 = my_malloc(64);
    void* block2 = my_malloc(128);
    void* block3 = my_malloc(256);
    my_free(block2, FALSE);
    my_free(block3, FALSE);
    check_heap();
    memory_usage();
    my_free(block1, TRUE);

    printf("%s\nTest: FREING WITH UNMAPING%s", YELLOW, RESET);
    void* block4 = my_malloc(32);
    void* block5 = my_malloc(48);
    printf("%s (Freing 32 bytes block at %p and unmaping memory)%s\n", GREEN, block4, RESET);
    my_free(block4, TRUE);
    check_heap();
    memory_usage();
    my_free(block5, TRUE);
}
