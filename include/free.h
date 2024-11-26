/**
 * @file free.h
 * @brief Header file for memory deallocation and validation functions.
 *
 * This header provides function prototypes for freeing allocated memory,
 * validating memory addresses, and merging adjacent free memory blocks.
 * These functions are part of the custom memory allocator.
 */

#include "../include/mem_logging.h" ///< Provides functionality for logging memory operations.
#include "../include/memory.h"      ///< Includes definitions for memory management structures and constants.

/**
 * @brief Merges a free memory block with adjacent free blocks.
 *
 * This function combines a given free block with its neighboring free blocks,
 * if available, to form a larger contiguous free block. The resulting block
 * is updated in the memory heap.
 *
 * @param b Pointer to the memory block to merge.
 * @return t_block Pointer to the merged memory block.
 */
t_block fusion(t_block b);

/**
 * @brief Checks if a given memory address is valid.
 *
 * Validates whether the provided pointer corresponds to a valid memory block
 * in the heap managed by the custom memory allocator.
 *
 * @param p Pointer to the memory address to validate.
 * @return int Returns 1 if the address is valid, 0 otherwise.
 */
int valid_addr(void* p);

/**
 * @brief Frees a previously allocated memory block.
 *
 * Marks the specified memory block as free, merges it with adjacent free blocks
 * if possible, and optionally unmaps it from the heap if it is the last block
 * and `unmap_flag` is enabled.
 *
 * @param ptr Pointer to the memory block to be freed.
 * @param unmap_flag Flag indicating whether to unmap the memory block if possible.
 */
void my_free(void* ptr, int unmap_flag);
