/**
 * @file mem_logging.h
 * @brief Header file for memory operation logging.
 *
 * This file defines structures, constants, and function prototypes for logging
 * memory allocation and deallocation operations in a custom memory management system.
 * It tracks memory usage and logs operations such as malloc, calloc, realloc, and free.
 */

#include <stdint.h> ///< Provides fixed-width integer types for consistency.
#include <stdio.h>  ///< Includes standard I/O functions for logging operations.
#include <string.h> ///< Provides memory manipulation functions like memset.
#include <sys/mman.h> ///< Includes memory mapping functions for dynamic memory management.
#include <time.h> ///< Provides utilities for time-stamping operations.

/**
 * @def LOG_FILE_PATH
 * Path to the log file where memory operations are recorded.
 */
#define LOG_FILE_PATH "/home/ignacio/iledesma/imem/memory_log.txt"

/**
 * @enum alloc_type
 * @brief Enumeration of memory operation types.
 *
 * Represents the type of memory operation being logged. Includes allocation,
 * reallocation, and deallocation operations.
 */
typedef enum
{
    MALLOC, ///< Memory allocated using malloc.
    CALLOC, ///< Memory allocated using calloc.
    REALLOC, ///< Memory reallocated using realloc.
    FREE ///< Memory deallocated using free.
} alloc_type;

/**
 * @struct log_entry
 * @brief Represents a log entry for a memory operation.
 *
 * A linked list structure that stores details of a memory operation, including
 * the type of operation, memory block pointer, size, and operation-specific data.
 */
typedef struct log_entry
{
    alloc_type type; ///< Type of the memory operation (e.g., MALLOC, FREE).
    void* ptr; ///< Pointer to the memory block involved in the operation.
    size_t size; ///< Size of the memory block.
    size_t total_allocated; ///< Total memory allocated at the time of the operation.
    size_t total_freed; ///< Total memory freed at the time of the operation.
    struct log_entry* next; ///< Pointer to the next log entry in the list.
    unsigned long op_id; ///< Unique identifier for the memory operation.
} t_log_entry;

/**
 * @brief Logs a memory operation.
 *
 * Creates a new log entry for a memory operation and adds it to the log list.
 * Each entry stores information about the operation type, memory block, size,
 * and current memory usage statistics.
 *
 * @param type The type of memory operation (e.g., MALLOC, FREE).
 * @param ptr Pointer to the memory block involved in the operation.
 * @param size Size of the memory block in bytes.
 * @param op_ctr Pointer to the operation counter, used to assign a unique ID.
 */
void log_mem_operation(alloc_type type, void* ptr, size_t size, unsigned long* op_ctr);
