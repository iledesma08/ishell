/**
 * @file calloc.h
 * @brief Header file for zero-initialized memory allocation.
 *
 * This header provides the prototype for the `calloc` function, which allocates
 * memory for an array of elements and initializes all bytes to zero. It leverages
 * the custom memory allocation system defined in the custom memory allocator.
 */

#include "../include/malloc.h" ///< Includes memory allocation functionality for custom allocator.
#include "../include/memory.h" ///< Includes definitions for memory management structures and constants.

/**
 * @brief Allocates memory for an array of elements and initializes all bytes to zero.
 *
 * This function calculates the total memory required for an array with the given
 * number of elements and element size, allocates the memory, and sets all bytes
 * in the allocated memory to zero.
 *
 * @param number Number of elements to allocate memory for.
 * @param size Size of each element in bytes.
 * @return void* Pointer to the allocated and zero-initialized memory, or NULL if the allocation fails.
 */
void* my_calloc(size_t number, size_t size);
