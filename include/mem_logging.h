/**
 * @file mem_logging.h
 * @brief Header file for memory operation logging.
 *
 * This file defines structures, constants, and function prototypes for logging
 * memory allocation and deallocation operations in a custom memory management system.
 * It tracks memory usage and logs operations such as malloc, calloc, realloc, and free.
 */

#pragma once // Prevents multiple inclusions of this header file during compilation.

#include <stdint.h>   ///< Provides fixed-width integer types for consistency.
#include <stdio.h>    ///< Includes standard I/O functions for logging operations.
#include <string.h>   ///< Provides memory manipulation functions like memset.
#include <sys/mman.h> ///< Includes memory mapping functions for dynamic memory management.
#include <time.h>     ///< Provides utilities for time-stamping operations.

/**
 * @def GRAY
 * ANSI escape code for gray-colored text in terminal output.
 */
#define GRAY "\033[1;30m"

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
 * @enum alloc_op
 * @brief Enumeration of memory operation types.
 *
 * Represents the type of memory operation being logged. Includes allocation,
 * reallocation, and deallocation operations.
 */
typedef enum
{
    MALLOC,  ///< Memory allocated using malloc.
    CALLOC,  ///< Memory allocated using calloc.
    REALLOC, ///< Memory reallocated using realloc.
    FREE     ///< Memory deallocated using free.
} alloc_op;

/**
 * @struct log_entry
 * @brief Represents a log entry for a memory operation.
 *
 * A linked list structure that stores details of a memory operation, including
 * the type of operation, memory block pointer, size, and operation-specific data.
 */
typedef struct log_entry
{
    alloc_op op;            ///< Type of the memory operation (e.g., MALLOC, FREE).
    void* ptr;              ///< Pointer to the memory block involved in the operation.
    size_t size;            ///< Size of the memory block.
    size_t total_allocated; ///< Total memory allocated at the time of the operation.
    size_t total_freed;     ///< Total memory freed at the time of the operation.
    struct log_entry* next; ///< Pointer to the next log entry in the list.
    unsigned long op_id;    ///< Unique identifier for the memory operation.
} t_log_entry;

/**
 * @brief Logs a memory operation.
 *
 * Creates a new log entry for a memory operation and adds it to the log list.
 * Each entry stores information about the operation type, memory block, size,
 * and current memory usage statistics.
 *
 * @param op The type of memory operation (e.g., MALLOC, FREE).
 * @param ptr Pointer to the memory block involved in the operation.
 * @param size Size of the memory block in bytes.
 * @param op_ctr Pointer to the operation counter, used to assign a unique ID.
 */
void log_mem_operation(alloc_op op, void* ptr, size_t size, unsigned long* op_ctr);

/**
 * @brief Clears all memory operation logs.
 *
 * Frees all log entries in the log list and resets the log state. This function
 * is used to clean up memory and remove all records of logged operations.
 */
void clear_logs(void);

/**
 * @brief Displays the memory operation log.
 *
 * Prints the memory operation log to the console, showing details of each
 * memory allocation and deallocation operation in chronological order.
 */
void show_logs(void);
