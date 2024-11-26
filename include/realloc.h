/**
 * @file realloc.h
 * @brief Header file for memory reallocation functions.
 *
 * This header defines the prototypes for functions that handle resizing
 * memory blocks and copying data between them. These functions are part
 * of the custom memory allocator and ensure efficient memory management.
 */

#pragma once // Prevents multiple inclusions of this header file during compilation.

#include "../include/free.h"   ///< Includes functionality for freeing memory blocks.
#include "../include/malloc.h" ///< Includes functionality for memory allocation.
#include "../include/memory.h" ///< Includes definitions for memory structures and constants.

/**
 * @brief Copies the contents of a source memory block to a destination block.
 *
 * Transfers the data from the source block to the destination block, ensuring
 * that no more data is copied than the size of either block. This function is
 * useful during memory reallocation when the block size changes.
 *
 * @param src Pointer to the source memory block.
 * @param dst Pointer to the destination memory block.
 */
void copy_block(t_block src, t_block dst);

/**
 * @brief Resizes a previously allocated memory block.
 *
 * Changes the size of an allocated memory block, preserving the existing data
 * up to the smaller of the old and new sizes. If the block needs to be moved,
 * a new block is allocated, and the old block's data is copied. The old block
 * is then freed.
 *
 * @param p Pointer to the memory block to be resized.
 * @param size New size in bytes for the memory block.
 * @return void* Pointer to the resized memory block, or NULL if reallocation fails.
 */
void* my_realloc(void* p, size_t size);
