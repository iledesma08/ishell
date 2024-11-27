#ifndef TEST_COMMANDS_H
#define TEST_COMMANDS_H

#include <unity/unity.h>

#include "../include/calloc.h"
#include "../include/free.h"
#include "../include/malloc.h"
#include "../include/realloc.h"

void setUp(void);

void tearDown(void);

void test_my_malloc_allocates_memory(void);

void test_my_free_frees_memory(void);

void test_my_calloc_zeroes_memory(void);

void test_my_realloc_increases_memory(void);

void test_split_block_creates_new_block(void);

void test_fusion_merges_adjacent_blocks(void);

void test_valid_addr_validates_pointer(void);

void test_extend_heap_adds_memory(void);

#endif // TEST_COMMANDS_H