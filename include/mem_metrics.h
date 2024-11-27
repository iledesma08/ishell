/**
 * @file mem_metrics.h
 * @brief Header file for memory allocator performance and fragmentation metrics.
 *
 * This file provides constants and function prototypes for calculating memory
 * fragmentation, measuring allocator efficiency, and managing the state of a custom
 * memory allocator. It includes functions for analyzing performance and fragmentation
 * for various memory allocation methods.
 */

#pragma once // Prevents multiple inclusions of this header file during compilation.

#include "../include/calloc.h"  ///< Includes functionality for zero-initialized memory allocation.
#include "../include/free.h"    ///< Includes functionality for freeing memory blocks.
#include "../include/malloc.h"  ///< Includes functionality for memory allocation.
#include "../include/memory.h"  ///< Includes core memory management structures and definitions.
#include "../include/realloc.h" ///< Includes functionality for memory reallocation.
#include <stdlib.h>             ///< Provides standard library functions like `exit`.

/**
 * @def ALLOCATION_SIZE_MAX
 * Maximum size for a single memory allocation in performance tests.
 */
#define ALLOCATION_SIZE_MAX 1024

/**
 * @def ALLOCATION_SIZE_MIN
 * Minimum size for a single memory allocation in performance tests.
 */
#define ALLOCATION_SIZE_MIN 8

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
 * @def MILLISECONDS_IN_SECOND
 * Defines the number of milliseconds in one second, used for time-related calculations.
 */
#define MILLISECONDS_IN_SECOND 1000.0

/**
 * @def NANOSECONDS_IN_MILLISECOND
 * Defines the number of nanoseconds in one millisecond, used for time-related calculations.
 */
#define NANOSECONDS_IN_MILLISECOND 1e6

/**
 * @brief Calculates memory fragmentation for each allocation method.
 *
 * Analyzes the memory heap to calculate external fragmentation for each supported
 * allocation method (e.g., First Fit, Best Fit, Worst Fit). Stores the calculated
 * fragmentation rates in the provided array.
 *
 * @param fragmentation_rates Array to store the fragmentation percentage for each method.
 */
void calculate_fragmentation_all_methods(double* fragmentation_rates);

/**
 * @brief Retrieves the current time in seconds.
 *
 * Combines seconds and nanoseconds into a single floating-point value to represent
 * the current time in seconds. Used for measuring performance metrics.
 *
 * @return double Current time in seconds.
 */
double get_time_in_milliseconds(void);

/**
 * @brief Runs efficiency tests for all supported allocation methods.
 *
 * Measures the time taken for memory allocations and deallocations, as well as
 * fragmentation rates, for all available allocation methods (e.g., First Fit, Best Fit, Worst Fit).
 */
void efficiency_test_all_methods(void);

/**
 * @brief Clears all memory blocks and resets the memory allocator.
 *
 * Frees all allocated blocks, clears fragmentation data, and resets the memory allocator
 * state. Ensures that no residual data remains in the heap.
 */
void clear_memory(void);
