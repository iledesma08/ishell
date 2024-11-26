#include "../include/free.h" // Include header for free-related functions and definitions.

extern void* base;                   // Pointer to the start of the heap.
extern pthread_mutex_t memory_mutex; // Mutex for thread-safe memory operations.
extern int enable_unmapping;         // Flag to enable or disable unmapping of memory blocks.
extern size_t total_freed_memory;    // Tracks the total memory freed by the allocator.
unsigned long free_ctr = 0;          // Counter for the number of free operations performed.

t_block fusion(t_block b)
{
    // Merge with subsequent free blocks.
    while (b->next && b->next->free)
    {
        t_block next_block = b->next;             // Get the next block.
        b->size += BLOCK_SIZE + next_block->size; // Increase the current block size.
        b->next = next_block->next;               // Update the link to skip the merged block.
        if (b->next)
            b->next->prev = b; // Update the previous pointer of the next block.
    }

    // Merge with previous free blocks.
    while (b->prev && b->prev->free)
    {
        t_block prev_block = b->prev;             // Get the previous block.
        prev_block->size += BLOCK_SIZE + b->size; // Increase the previous block size.
        prev_block->next = b->next;               // Update the link to skip the merged block.
        if (b->next)
            b->next->prev = prev_block; // Update the previous pointer of the next block.
        b = prev_block;                 // Move to the previous block as the new reference.
    }
    return b; // Return the fused block.
}

int valid_addr(void* p)
{
    if (p == NULL || base == NULL) // Check if the pointer or heap base is null.
    {
        return 0; // Invalid address.
    }
    t_block b = get_block(p); // Retrieve the block metadata for the pointer.
    t_block current = base;   // Start checking from the base of the heap.

    // Traverse the heap to find the block.
    while (current)
    {
        if (current == b) // If the block matches the metadata.
        {
            return (current->ptr == p); // Validate that the pointer matches the block's data pointer.
        }
        current = current->next; // Move to the next block.
    }
    return INVALID_ADDRESS; // Return invalid if the block is not found.
}

void my_free(void* ptr, int unmap_flag)
{
    pthread_mutex_lock(&memory_mutex); // Lock the mutex for thread safety.

    if (ptr == NULL) // If the pointer is null, there's nothing to free.
    {
        pthread_mutex_unlock(&memory_mutex); // Unlock the mutex before returning.
        return;
    }

    t_block b;

    if (valid_addr(ptr)) // Check if the pointer is valid.
    {
        b = get_block(ptr); // Retrieve the block metadata.
        b->free = TRUE;     // Mark the block as free.

        // Merge with adjacent free blocks.
        b = fusion(b);

        // Update the total freed memory.
        total_freed_memory += b->size;

        // Log the free operation.
        log_mem_operation(FREE, ptr, b->size, &free_ctr);

        // If unmap_flag is set and the block is at the end of the heap.
        if (unmap_flag && b->next == NULL)
        {
            if (b->prev) // If there is a previous block, update its next pointer.
            {
                b->prev->next = NULL;
            }
            else
            {
                base = NULL; // If this is the only block, reset the heap base.
            }

            // Check if the block should be unmapped.
            if (enable_unmapping && b->free)
            {
                size_t total_size = b->size + BLOCK_SIZE; // Calculate the total size of the block.
                printf("Unmapping block at %p with size: %zu\n", (void*)b, total_size);

                // Attempt to unmap the block from memory.
                if (munmap(b, total_size) == -1)
                {
                    perror("munmap"); // Print an error if unmapping fails.
                }
                else
                {
                    // Reset the base pointer if it was pointing to the unmapped block.
                    if (base == b)
                    {
                        base = NULL;
                    }
                }
            }
        }
    }
    else
    {
        // Print an error message for an invalid pointer.
        printf("Failed attempt to free - %sInvalid Address%s: %p\n", RED, RESET, ptr);
    }

    pthread_mutex_unlock(&memory_mutex); // Unlock the mutex after completing the operation.
}
