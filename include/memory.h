/**
 * @file memory.h
 * @brief Custom memory management library.
 *
 * This header file defines the structures, constants, and function prototypes
 * required for implementing a custom memory allocator. It provides tools for
 * dynamic memory allocation, freeing, and diagnostic checks of the heap to ensure
 * efficient memory usage.
 */

#pragma once // Prevents multiple inclusions of this header file during compilation.

/* Required headers for memory management */
#include "mem_logging.h" ///< Provides functionality for logging memory operations.
#include <pthread.h>     ///< Enables thread safety with mutex support.
#include <stddef.h>      ///< Defines standard types like `size_t`.
#include <stdint.h>      ///< Provides fixed-width integer types.
#include <stdio.h>       ///< Includes I/O utilities for debugging.
#include <string.h>      ///< Includes memory manipulation utilities like `memset`.
#include <sys/mman.h>    ///< Provides memory mapping functions for dynamic memory allocation.
#include <sys/types.h>   ///< Defines basic data types.
#include <unistd.h>      ///< Provides POSIX API functions for memory management.

/**
 * @def INVALID_ADDRESS
 * Indicates that a memory address is invalid or inaccessible.
 */
#define INVALID_ADDRESS 0

/**
 * @def ALIGNMENT
 * Minimum padding required for block alignment.
 */
#define ALIGNMENT 8

/**
 * @def align
 * Aligns a size value to the next multiple of 8 bytes.
 *
 * Ensures memory allocations conform to alignment requirements, which
 * can improve performance and prevent hardware errors.
 *
 * @param x The size to align.
 * @return The aligned size.
 */
#define align(x) (((((x)-1) >> 3) << 3) + ALIGNMENT)

/**
 * @def BLOCK_MIN_SIZE
 * The minimum size of a memory block, including metadata.
 */
#define BLOCK_MIN_SIZE 40

/**
 * @def PAGESIZE
 * Defines the size of a memory page, typically used in mmap allocations.
 */
#define PAGESIZE 4096

/**
 * @def FIRST_FIT
 * Memory allocation strategy: finds the first free block that fits the request.
 */
#define FIRST_FIT 0

/**
 * @def BEST_FIT
 * Memory allocation strategy: finds the smallest block that fits the request.
 */
#define BEST_FIT 1

/**
 * @def WORST_FIT
 * Memory allocation strategy: finds the largest available block.
 */
#define WORST_FIT 2

/**
 * @def ALLOC_METHODS
 * Total number of memory allocation strategies supported.
 */
#define ALLOC_METHODS 3

/**
 * @def DATA_START
 * Offset for the start of the data section in a memory block.
 */
#define DATA_START 1

/**
 * @def TRUE
 * Boolean true value, used for readability in memory block operations.
 */
#define TRUE 1

/**
 * @def FALSE
 * Boolean false value, used for readability in memory block operations.
 */
#define FALSE 0

/**
 * @brief t_block
 * Alias for a pointer to the `s_block` structure, representing a memory block.
 */
typedef struct s_block* t_block;

/**
 * @brief Represents a block of memory managed by the allocator.
 *
 * This structure contains metadata about a memory block, including its size,
 * pointers to adjacent blocks, and allocation status. It is part of a linked
 * list that enables the allocator to manage the heap efficiently.
 */
struct s_block
{
    size_t size;           /**< Size of the memory block's data area. */
    struct s_block* next;  /**< Pointer to the next block in the heap. */
    struct s_block* prev;  /**< Pointer to the previous block in the heap. */
    int free;              /**< Status flag: 1 if the block is free, 0 if allocated. */
    void* ptr;             /**< Pointer to the start of the data section. */
    int alloc_method;      /**< Allocation strategy used for this block. */
    char data[DATA_START]; /**< Start of the block's data section. */
};

/**
 * @brief Initializes the memory manager.
 *
 * Sets up the necessary data structures and locks for managing memory allocations.
 */
void memory_manager_init(void);

/**
 * @brief Cleans up resources used by the memory manager.
 *
 * Releases any locks or resources held by the memory manager, preparing it for shutdown.
 */
void memory_manager_cleanup(void);

/**
 * @brief Retrieves the metadata block for a given data pointer.
 *
 * Computes the starting address of the metadata structure that corresponds to
 * the given data pointer, enabling access to block metadata for allocation or deallocation.
 *
 * @param p Pointer to a data block allocated by the custom allocator.
 * @return t_block Pointer to the metadata block associated with the data pointer.
 */
t_block get_block(void* p);

/**
 * @brief Sets the allocation method for the memory manager.
 *
 * Configures the allocation strategy used by the memory manager. Supported
 * strategies include First Fit, Best Fit, and Worst Fit.
 *
 * @param m Allocation method to set (FIRST_FIT, BEST_FIT, or WORST_FIT).
 */
void set_method(int m);

/**
 * @brief Performs a diagnostic check of the heap.
 *
 * Scans the heap for issues such as adjacent free blocks that can be merged or
 * invalid block sizes. Outputs diagnostic information for debugging purposes.
 */
void check_heap(void);

/**
 * @brief Displays memory usage statistics.
 *
 * Prints details about the total allocated and freed memory, as well as the
 * current state of the heap.
 */
void memory_usage(void);
