#include "heap_test.h"   // Include the header for heap test functionality.
#include "memory_test.h" // Include the header for memory test functionality.

int main(void)
{
    printf("\n%s====== Running Empty CHECK_HEAP Test ======%s\n", BLUE, RESET);
    check_heap(); // Test empty heap
    test_memory_usage();
    test_check_heap();
    printf("\n%s--------------------------------%s\n", BLUE, RESET);
    show_logs();
    printf("\n%s====== Running EFFICIENCY and FRAGMENTATION Tests ======%s\n\n", BLUE, RESET);
    efficiency_test_all_methods();
    return 0;
}
