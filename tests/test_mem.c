#include "test_mem.h"

// Mock global variables
extern void* base;
extern int method;
extern pthread_mutex_t memory_mutex;
extern size_t total_allocated_memory;
extern size_t total_freed_memory;
extern unsigned long malloc_ctr;
extern unsigned long free_ctr;
extern unsigned long calloc_ctr;
extern unsigned long realloc_ctr;

void setUp(void)
{
    // Reset global state before each test
    base = NULL;
    total_allocated_memory = 0;
    total_freed_memory = 0;
    malloc_ctr = 0;
    free_ctr = 0;
    calloc_ctr = 0;
    realloc_ctr = 0;
}

void tearDown(void)
{
    // Clean up global state after each test
    if (base)
    {
        munmap(base, total_allocated_memory);
        base = NULL;
    }
}

// Test for `my_malloc`
void test_my_malloc_allocates_memory(void)
{
    size_t size = 64;
    void* ptr = my_malloc(size);

    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(size, total_allocated_memory);

    my_free(ptr, FALSE); // Clean up
}

// Test for `my_free`
void test_my_free_frees_memory(void)
{
    size_t size = 64;
    void* ptr = my_malloc(size);

    my_free(ptr, FALSE);

    TEST_ASSERT_EQUAL(size, total_freed_memory);
}

// Test for `my_calloc`
void test_my_calloc_zeroes_memory(void)
{
    size_t count = 10;
    size_t size = 4;
    void* ptr = my_calloc(count, size);

    printf("test_my_calloc_zeroes_memory\n");

    TEST_ASSERT_NOT_NULL(ptr);

    char* data = (char*)ptr;
    for (size_t i = 0; i < count * size; i++)
    {
        TEST_ASSERT_EQUAL(0, data[i]);
    }

    my_free(ptr, FALSE); // Clean up
}

// Test for `my_realloc`
void test_my_realloc_increases_memory(void)
{
    size_t initial_size = 64;
    size_t new_size = 128;
    void* ptr = my_malloc(initial_size);

    void* new_ptr = my_realloc(ptr, new_size);

    TEST_ASSERT_NOT_NULL(new_ptr);
    TEST_ASSERT_EQUAL(initial_size + new_size, total_allocated_memory);

    my_free(new_ptr, FALSE); // Clean up
}

// Test for `split_block`
void test_split_block_creates_new_block(void)
{
    size_t size = 128;
    void* ptr = my_malloc(size);

    t_block block = get_block(ptr);
    size_t split_size = 64;
    split_block(block, split_size);

    TEST_ASSERT_EQUAL(split_size, block->size);
    TEST_ASSERT_NOT_NULL(block->next);
    TEST_ASSERT_EQUAL(size - split_size - BLOCK_MIN_SIZE, block->next->size);

    my_free(ptr, FALSE); // Clean up
}

// Test for `fusion`
void test_fusion_merges_adjacent_blocks(void)
{
    size_t size = 64;
    void* ptr1 = my_malloc(size);
    void* ptr2 = my_malloc(size);

    my_free(ptr1, FALSE);
    my_free(ptr2, FALSE);

    t_block block = get_block(ptr1);
    block = fusion(block);

    TEST_ASSERT_EQUAL(size * 2 + BLOCK_MIN_SIZE, block->size);
}

// Test for `valid_addr`
void test_valid_addr_validates_pointer(void)
{
    size_t size = 64;
    void* ptr = my_malloc(size);

    TEST_ASSERT_TRUE(valid_addr(ptr));
    TEST_ASSERT_FALSE(valid_addr((void*)0x12345678)); // Invalid address

    my_free(ptr, FALSE); // Clean up
}

// Test for `extend_heap`
void test_extend_heap_adds_memory(void)
{
    size_t size = 128;
    t_block block = extend_heap(NULL, size);

    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_EQUAL(size, block->size);

    munmap(block, size + BLOCK_MIN_SIZE); // Clean up
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_my_malloc_allocates_memory);
    RUN_TEST(test_my_free_frees_memory);
    RUN_TEST(test_my_calloc_zeroes_memory);
    RUN_TEST(test_my_realloc_increases_memory);
    RUN_TEST(test_split_block_creates_new_block);
    RUN_TEST(test_fusion_merges_adjacent_blocks);
    RUN_TEST(test_valid_addr_validates_pointer);
    RUN_TEST(test_extend_heap_adds_memory);

    return UNITY_END();
}
