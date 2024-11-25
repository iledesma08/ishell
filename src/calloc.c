#include "include/calloc.h"

// Function to allocate zero-initialized memory
void* calloc(size_t number, size_t size)
{
    // Prevent integer overflow when multiplying number and size
    if (number == 0 || size == 0 || number > SIZE_MAX / size)
    {
        return NULL; // If overflow, return NULL to indicate allocation failure
    }

    size_t total_size = number * size;

    // Allocate memory using malloc
    void* new = malloc(total_size);
    if (new)
    {
        // Set allocated memory to zero using memset for better efficiency
        memset(new, 0, total_size);
    }
    return new; // Return the allocated and zero-initialized memory
}