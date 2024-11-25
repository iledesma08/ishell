#include "../include/memory.h" // Custom memory management header

// Typedef for memory block structure
typedef struct s_block* t_block;
void* base = NULL; // Pointer to the beginning of the heap
int method = 0;    // Memory allocation method (0 = First Fit, 1 = Best Fit)

// Function to find a suitable memory block based on the allocation method
t_block find_block(t_block* last, size_t size)
{
    t_block b = base;

    if (method == FIRST_FIT)
    { // First Fit strategy
        while (b && !(b->free && b->size >= size))
        {                // Look for the first block large enough and free
            *last = b;   // Update the last block traversed
            b = b->next; // Move to the next block
        }
        return (b); // Return the found block or NULL
    }
    else
    {                          // Best Fit strategy
        size_t dif = PAGESIZE; // Difference between the requested size and the block size
        t_block best = NULL;   // Pointer to the best-fit block

        while (b)
        {
            if (b->free)
            { // If the block is free
                if (b->size == size)
                { // Perfect match
                    return b;
                }
                if (b->size > size && (b->size - size) < dif)
                { // Smaller difference is better
                    dif = b->size - size;
                    best = b;
                }
            }
            *last = b;   // Update the last traversed block
            b = b->next; // Move to the next block
        }
        return best; // Return the best-fit block or NULL
    }
}

// Function to split a memory block into two
void split_block(t_block b, size_t s)
{
    if (b->size <= s + BLOCK_SIZE)
    { // Check if the block can be split
        return;
    }

    t_block new;                          // Create a new block for the leftover memory
    new = (t_block)(b->data + s);         // Assign the new block after the allocated space
    new->size = b->size - s - BLOCK_SIZE; // Adjust size for the new block
    new->next = b->next;                  // Link the new block to the next block
    new->free = 1;                        // Mark the new block as free
    b->size = s;                          // Resize the original block
    b->next = new;                        // Update the next link of the original block
}

// Function to copy data from one block to another
void copy_block(t_block src, t_block dst)
{
    int *sdata, *ddata;
    size_t i;

    sdata = src->ptr; // Source data pointer
    ddata = dst->ptr; // Destination data pointer

    // Copy data from the source block to the destination block
    for (i = 0; (i * 4) < src->size && (i * 4) < dst->size; i++)
    {
        ddata[i] = sdata[i];
    }
}

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

// Function to check if a pointer is valid and part of the heap
int valid_addr(void* p)
{
    if (base)
    { // Ensure the heap exists
        if (p > base && p < sbrk(0))
        {                              // Check if the pointer is within the heap range
            t_block b = get_block(p);  // Get the block associated with the pointer
            return b && (p == b->ptr); // Validate the pointer
        }
    }
    return (0); // Invalid pointer
}

// Function to merge adjacent free memory blocks
t_block fusion(t_block b)
{
    if (b->next && b->next->free)
    {                                          // Check if the next block is free
        b->size += BLOCK_SIZE + b->next->size; // Merge the sizes
        b->next = b->next->next;               // Skip the merged block
        if (b->next)
        {
            b->next->prev = b; // Update the previous pointer of the next block
        }
    }
    return b;
}

// Function to extend the heap by creating a new block
t_block extend_heap(t_block last, size_t s)
{
    t_block b;
    b = mmap(0, s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); // Allocate memory with mmap

    if (b == MAP_FAILED)
    { // Check if mmap failed
        return NULL;
    }
    b->size = s;      // Set the size of the block
    b->next = NULL;   // No next block
    b->prev = last;   // Link to the last block
    b->ptr = b->data; // Pointer to the block's data section

    if (last)
    {
        last->next = b; // Update the next link of the last block
    }
    b->free = 0; // Mark the block as allocated
    return b;
}

// Function to set the allocation method
void get_method(int m)
{
    method = m;
}

// Function to set the allocation method
void set_method(int m)
{
    method = m;
}

// Function to control allocation strategies
void malloc_control(int m)
{
    if (m == 0)
    {
        set_method(0); // First Fit
    }
    else if (m == 1)
    {
        set_method(1); // Best Fit
    }
    else
    {
        printf("Error: invalid method\n"); // Handle invalid methods
    }
}

// Function to allocate memory
void* malloc(size_t size)
{
    t_block b, last;
    size_t s = align(size); // Align the requested size

    if (base)
    { // If the heap exists
        last = base;
        b = find_block(&last, s); // Find a suitable block
        if (b)
        {
            if ((b->size - s) >= (BLOCK_SIZE + 4))
            {
                split_block(b, s); // Split the block if there's excess space
            }
            b->free = 0; // Mark the block as allocated
        }
        else
        {
            b = extend_heap(last, s); // Extend the heap if no block is found
            if (!b)
                return NULL;
        }
    }
    else
    { // Initialize the heap
        b = extend_heap(NULL, s);
        if (!b)
            return NULL;
        base = b; // Set the base pointer
    }
    return b->data; // Return a pointer to the allocated space
}

// Function to free allocated memory
void free(void* ptr)
{
    t_block b;

    if (valid_addr(ptr))
    {                       // Ensure the pointer is valid
        b = get_block(ptr); // Get the block associated with the pointer
        b->free = 1;        // Mark the block as free

        if (b->next && b->next->free)
        {
            fusion(b); // Merge with the next block if it's free
        }
        if (b->prev && b->prev->free)
        {
            fusion(b->prev); // Merge with the previous block if it's free
        }
    }
}

// Function to allocate zero-initialized memory
void* calloc(size_t number, size_t size)
{
    size_t* new;
    size_t s4, i;

    if (!number || !size)
    {
        return NULL;
    }
    new = malloc(number * size); // Allocate memory
    if (new)
    {
        s4 = align(number * size) << 2;
        for (i = 0; i < s4; i++)
        {
            new[i] = 0; // Zero-initialize the memory
        }
    }
    return new;
}

// Function to reallocate memory
void* realloc(void* ptr, size_t size)
{
    size_t s;
    t_block b, new;
    void* newp;

    if (!ptr)
        return malloc(size); // If the pointer is NULL, act like malloc

    if (valid_addr(ptr))
    { // Ensure the pointer is valid
        s = align(size);
        b = get_block(ptr);

        if (b->size >= s)
        { // If the block is already large enough
            if (b->size - s >= (BLOCK_SIZE + 4))
            {
                split_block(b, s); // Resize the block
            }
        }
        else
        {
            if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
            {
                fusion(b); // Merge with the next block if possible
                if (b->size - s >= (BLOCK_SIZE + 4))
                {
                    split_block(b, s);
                }
            }
            else
            {
                newp = malloc(s); // Allocate a new block
                if (!newp)
                    return NULL;
                new = get_block(newp);
                copy_block(b, new); // Copy data to the new block
                free(ptr);          // Free the old block
                return newp;
            }
        }
        return ptr;
    }
    return NULL;
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
