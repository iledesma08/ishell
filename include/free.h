#include "include/memory.h"

/**
 * @brief Fusiona un bloque libre con su siguiente bloque si también está libre.
 *
 * @param b Bloque a fusionar.
 * @return t_block Puntero al bloque fusionado.
 */
t_block fusion(t_block b);

/**
 * @brief Obtiene el bloque que contiene una dirección de memoria dada.
 *
 * @param p Puntero a la dirección de datos.
 * @return t_block Puntero al bloque de memoria correspondiente.
 */
t_block get_block(void* p);

/**
 * @brief Verifica si una dirección de memoria es válida.
 *
 * @param p Dirección de memoria a verificar.
 * @return int Retorna 1 si la dirección es válida, 0 en caso contrario.
 */
int valid_addr(void* p);

/**
 * @brief Libera un bloque de memoria previamente asignado.
 *
 * @param p Puntero al área de datos a liberar.
 */
void free(void* p);