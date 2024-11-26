/**
 * @file memory.h
 * @brief Memory management library with custom allocation functions.
 *
 * Esta biblioteca define funciones de asignación de memoria dinámica
 * y gestión de bloques para crear un asignador de memoria personalizado.
 */

#pragma once

#include "mem_logging.h"
#include <pthread.h> // Include for mutexes
#include <stddef.h>
#include <stdint.h> // Include limits for overflow checks
#include <stdio.h>
#include <string.h>
#include <string.h>   // Include for memset function
#include <sys/mman.h> // Memory management functions (e.g., mmap)
#include <sys/types.h>
#include <unistd.h>
#include <linux/time.h>

#define INVALID_ADDRESS -1
#define BLOCK_THRESHOLD 8

/**
 * @brief Macro para alinear una cantidad de bytes al siguiente múltiplo de 8.
 *
 * @param x Cantidad de bytes a alinear.
 */
#define align(x) (((((x) - 1) >> 3) << 3) + BLOCK_THRESHOLD)

/** Tamaño mínimo de un bloque de memoria. */
#define BLOCK_SIZE 40
/** Tamaño de página en memoria. */
#define PAGESIZE 4096
/** Política de asignación First Fit. */
#define FIRST_FIT 0
/** Política de asignación Best Fit. */
#define BEST_FIT 1
/** Politica de asignacion Worst Fit */
#define WORST_FIT 2
/** Tamaño del bloque */
#define DATA_START 1
#define TRUE 1
#define FALSE 0

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

/** Tipo de puntero para un bloque de memoria. */
typedef struct s_block* t_block;

/**
 * @struct s_block
 * @brief Estructura para representar un bloque de memoria.
 *
 * Contiene la información necesaria para gestionar la asignación y
 * liberación de un bloque de memoria.
 */
struct s_block
{
    size_t size;           /**< Tamaño del bloque de datos. */
    struct s_block* next;  /**< Puntero al siguiente bloque en la lista enlazada. */
    struct s_block* prev;  /**< Puntero al bloque anterior en la lista enlazada. */
    int free;              /**< Indicador de si el bloque está libre (1) o ocupado (0). */
    void* ptr;             /**< Puntero a la dirección de los datos almacenados. */
    int alloc_method;      /**< Método de asignación utilizado para el bloque. */
    char data[DATA_START]; /**< Área donde comienzan los datos del bloque. */
};

/**
 * @brief Obtiene el bloque que contiene una dirección de memoria dada.
 *
 * @param p Puntero a la dirección de datos.
 * @return t_block Puntero al bloque de memoria correspondiente.
 */
t_block get_block(void* p);

/**
 * @brief Verifica el estado del heap y detecta bloques libres consecutivos.
 *
 * @param data Información adicional para la verificación.
 */
void check_heap(void);
