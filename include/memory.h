/**
 * @file memory.h
 * @brief Custom memory management library.
 *
 * This header defines the structures, constants, and function prototypes
 * needed for a custom memory allocator. It provides tools for dynamic
 * memory allocation, freeing, and diagnostic checks of the heap.
 */

#pragma once // Prevents the header file from being included multiple times in the same compilation unit.

/* Required headers for memory management */
#include "mem_logging.h" ///< For logging memory operations.
#include <linux/time.h>  ///< Provides time-related utilities.
#include <pthread.h>     ///< Enables thread safety with mutexes.
#include <stddef.h>      ///< Standard definitions like `size_t`.
#include <stdint.h>      ///< Fixed-width integer types.
#include <stdio.h>       ///< Input/output utilities.
#include <string.h>      ///< Memory manipulation functions like `memset`.
#include <sys/mman.h>    ///< Memory management functions, such as `mmap`.
#include <sys/types.h>   ///< Basic data types.
#include <unistd.h>      ///< Access to POSIX API functions.

/**
 * @def INVALID_ADDRESS
 * Constant used to indicate that a memory address is invalid.
 */
#define INVALID_ADDRESS -1

/**
 * @def BLOCK_THRESHOLD
 * Defines the minimum padding needed for block alignment.
 */
#define BLOCK_THRESHOLD 8

/**
 * @def align
 * Aligns a size value to the next multiple of 8 bytes.
 *
 * This macro ensures memory allocations conform to system alignment requirements,
 * improving access performance and avoiding potential errors on certain architectures.
 *
 * @param x The size to align.
 * @return The aligned size value.
 */
#define align(x) (((((x) - 1) >> 3) << 3) + BLOCK_THRESHOLD)

/**
 * @def BLOCK_SIZE
 * The minimum size of a memory block, including metadata.
 */
#define BLOCK_SIZE 40

/**
 * @def PAGESIZE
 * The size of a memory page, typically used for mmap allocations.
 */
#define PAGESIZE 4096

/**
 * @def FIRST_FIT
 * Allocation strategy: assign the first free block large enough for the requested size.
 */
#define FIRST_FIT 0

/**
 * @def BEST_FIT
 * Allocation strategy: assign the smallest free block that fits the requested size.
 */
#define BEST_FIT 1

/**
 * @def WORST_FIT
 * Allocation strategy: assign the largest free block available.
 */
#define WORST_FIT 2

/**
 * @def ALLOC_METHODS
 * The total number of supported allocation strategies.
 */
#define ALLOC_METHODS 3

/**
 * @def DATA_START
 * Offset for the start of the data section in a memory block.
 */
#define DATA_START 1

/**
 * @def TRUE
 * Boolean true value.
 */
#define TRUE 1

/**
 * @def FALSE
 * Boolean false value.
 */
#define FALSE 0

/**
 * @def RED
 * ANSI escape code for red-colored text in terminal output.
 */
#define RED "\033[1;31m"

/**
 * @def GREEN
 * ANSI escape code for green-colored text in terminal output.
 */
#define GREEN "\033[1;32m"

/**
 * @def YELLOW
 * ANSI escape code for yellow-colored text in terminal output.
 */
#define YELLOW "\033[1;33m"

/**
 * @def BLUE
 * ANSI escape code for blue-colored text in terminal output.
 */
#define BLUE "\033[1;34m"

/**
 * @def RESET
 * ANSI escape code to reset terminal text formatting.
 */
#define RESET "\033[0m"

/**
 * @typedef t_block
 * Alias for a pointer to the `s_block` structure.
 */
typedef struct s_block* t_block;

/**
 * @struct s_block
 * @brief Represents a block of memory in the custom allocator.
 *
 * Each block contains metadata and a pointer to its data area. The linked
 * structure enables the allocator to manage dynamic memory efficiently.
 */
struct s_block
{
    size_t size;           /**< The size of the data area in bytes. */
    struct s_block* next;  /**< Pointer to the next block in the linked list. */
    struct s_block* prev;  /**< Pointer to the previous block in the linked list. */
    int free;              /**< Status flag: 1 if the block is free, 0 if allocated. */
    void* ptr;             /**< Pointer to the start of the data area. */
    int alloc_method;      /**< The allocation strategy used for this block. */
    char data[DATA_START]; /**< Start of the data section. Actual size is dynamic. */
};

/**
 * @brief Retrieves the metadata block for a given data pointer.
 *
 * This function calculates the starting address of the block structure
 * that precedes the given data pointer. It is critical for managing
 * allocated blocks and ensuring proper alignment.
 *
 * @param p A pointer to a data segment allocated by the custom allocator.
 * @return t_block A pointer to the corresponding metadata block.
 */
t_block get_block(void* p);

/**
 * @brief Analyzes the state of the heap and logs potential issues.
 *
 * This function scans all memory blocks in the heap, checking for
 * abnormalities like adjacent free blocks that could be merged or
 * blocks with invalid sizes. It provides detailed output for debugging.
 */
void check_heap(void);
