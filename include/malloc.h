/**
 * @file malloc.h
 * @brief Header file for custom memory allocation functions.
 *
 * This header defines the function prototypes for memory allocation,
 * including finding, splitting, and expanding memory blocks, as well
 * as allocating a block of memory using the custom allocator.
 */

#pragma once // Prevents multiple inclusions of this header file during compilation.

#include "mem_logging.h" ///< Includes functionality for logging memory operations.
#include "memory.h"      ///< Includes custom memory management definitions.

/**
 * @brief Finds a free memory block that can fit the requested size.
 *
 * Searches the heap for a free block of memory that is at least as large
 * as the requested size. If a suitable block is found, it is returned.
 * Otherwise, NULL is returned.
 *
 * @param last Pointer to the last block traversed during the search. Updated to point to the final block checked.
 * @param size Requested size in bytes.
 * @return t_block Pointer to the found memory block, or NULL if no suitable block is available.
 */
t_block find_block(t_block* last, size_t size);

/**
 * @brief Splits a memory block into two if it is larger than the requested size.
 *
 * Adjusts the size of the current block to match the requested size, and creates
 * a new block with the remaining space. The new block is marked as free and linked
 * to the current block.
 *
 * @param b Pointer to the block to split.
 * @param s Size of the new block in bytes.
 */
void split_block(t_block b, size_t s);

/**
 * @brief Expands the heap by creating a new memory block.
 *
 * Extends the heap by allocating a new block of memory and linking it to the
 * existing heap structure. If the allocation fails, NULL is returned.
 *
 * @param last Pointer to the last block in the current heap.
 * @param s Size of the new block in bytes.
 * @return t_block Pointer to the newly created block, or NULL if allocation fails.
 */
t_block extend_heap(t_block last, size_t s);

/**
 * @brief Allocates a block of memory of the requested size.
 *
 * Attempts to find a suitable free block in the heap or extends the heap if
 * no suitable block is available. Returns a pointer to the allocated memory,
 * or NULL if the allocation fails.
 *
 * @param size Requested size in bytes for the memory block.
 * @return void* Pointer to the allocated memory block, or NULL if allocation fails.
 */
void* my_malloc(size_t size);
