/**
 * @file mem_metrics.h
 * @brief Header file for memory allocator performance and fragmentation metrics.
 *
 * This file provides constants and function prototypes for calculating memory
 * fragmentation, measuring allocator efficiency, and managing the state of a custom
 * memory allocator.
 */

#include "../include/memory.h" ///< Includes core memory management structures and definitions.
#include "../include/malloc.h" ///< Includes functionality for memory allocation.
#include "../include/calloc.h" ///< Includes functionality for zero-initialized memory allocation.
#include "../include/realloc.h" ///< Includes functionality for memory reallocation.
#include "../include/free.h" ///< Includes functionality for freeing memory blocks.

/**
 * @def ALLOCATION_SIZE_MAX
 * Maximum size for a single memory allocation in performance tests.
 */
#define ALLOCATION_SIZE_MAX 1024

/**
 * @def ALLOCATION_SIZE_MIN
 * Minimum size for a single memory allocation in performance tests.
 */
#define ALLOCATION_SIZE_MIN 1

/**
 * @def NUM_ALLOCATIONS
 * Number of memory allocations to perform during the efficiency test.
 */
#define NUM_ALLOCATIONS 1000

/**
 * @def TO_PERCENTAGE_MULTIPLIER
 * Multiplier used to convert fragmentation values to percentages.
 */
#define TO_PERCENTAGE_MULTIPLIER 100

/**
 * @def NANOSECONDS_IN_SECOND
 * Defines the number of nanoseconds in one second, used for time-related calculations.
 */
#define NANOSECONDS_IN_SECOND 1000000000
