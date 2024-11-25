#include "../include/memory.h" // Custom memory management header

void* base = NULL; // Pointer to the beginning of the heap
int method = 0;    // Memory allocation method (0 = First Fit, 1 = Best Fit)

// Function to get the block associated with a pointer
t_block get_block(void* p)
{
    char* tmp = p; // Convert to a character pointer

    if (tmp >= (char*)base + BLOCK_SIZE)
    {                      // Ensure the pointer is within the heap range
        tmp -= BLOCK_SIZE; // Move to the block metadata
    }
    return (t_block)(tmp); // Return the block
}

// Function to check and display details of a block
void check_heap(void* data)
{
    if (data == NULL)
    {
        printf("Data is NULL\n");
        return;
    }

    t_block block = get_block(data);

    if (block == NULL)
    {
        printf("Block is NULL\n");
        return;
    }

    printf("\033[1;33mHeap check\033[0m\n");
    printf("Size: %zu\n", block->size);

    if (block->next != NULL)
    {
        printf("Next block: %p\n", (void*)(block->next));
    }
    else
    {
        printf("Next block: NULL\n");
    }

    if (block->prev != NULL)
    {
        printf("Prev block: %p\n", (void*)(block->prev));
    }
    else
    {
        printf("Prev block: NULL\n");
    }

    printf("Free: %d\n", block->free);

    if (block->ptr != NULL)
    {
        printf("Beginning data address: %p\n", block->ptr);
        printf("Last data address: %p\n", (void*)((char*)(block->ptr) + block->size));
    }
    else
    {
        printf("Data address: NULL\n");
    }

    printf("Heap address: %p\n", sbrk(0));
}
