#include "../include/memory.h"
#include "../include/malloc.h"
#include "../include/calloc.h"
#include "../include/realloc.h"
#include "../include/free.h"

#define ALLOCATION_SIZE_MAX 1024 // Define a suitable maximum allocation size
#define ALLOCATION_SIZE_MIN 1    // Define a suitable minimum allocation size
#define NUM_ALLOCATIONS 1000      // Define the number of allocations for the test
#define TO_PERCENTAGE_MULTIPLIER 100 // Multiplier to convert to percentage