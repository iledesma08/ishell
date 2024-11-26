#include "../include/free.h"

extern void* base; // Base of the heap
extern pthread_mutex_t memory_mutex; // Mutex to synchronize memory operations
extern int enable_unmapping;
extern size_t total_freed_memory;
unsigned long free_ctr = 0;

t_block fusion(t_block b)
{
    // Fuse with next blocks
    while (b->next && b->next->free)
    {
        t_block next_block = b->next;
        b->size += BLOCK_SIZE + next_block->size;
        b->next = next_block->next;
        if (b->next)
            b->next->prev = b;
    }

    // Fuse with previous blocks
    while (b->prev && b->prev->free)
    {
        t_block prev_block = b->prev;
        prev_block->size += BLOCK_SIZE + b->size;
        prev_block->next = b->next;
        if (b->next)
            b->next->prev = prev_block;
        b = prev_block;
    }
    return b;
}

// Function to check if a pointer is valid and part of the heap
int valid_addr(void* p)
{
    if (p == NULL || base == NULL)
    {
        return 0;
    }
    t_block b = get_block(p);
    t_block current = base;
    while (current)
    {
        if (current == b)
        {
            return (current->ptr == p);
        }
        current = current->next;
    }
    return INVALID_ADDRESS;
}

void free(void *ptr, int unmap_flag) {
  pthread_mutex_lock(&memory_mutex);
  if (ptr == NULL) {
    pthread_mutex_unlock(&memory_mutex);
    return;
  }

  t_block b;

  if (valid_addr(ptr)) {
    b = get_block(ptr);
    b->free = TRUE;

    // Fuse with adjacent free blocks
    b = fusion(b);
    total_freed_memory += b->size;
    add_log_entry(FREE, ptr, b->size);

    if (unmap_flag && b->next == NULL) {
      if (b->prev) {
        b->prev->next = NULL;
      } else {
        base = NULL; // This was the base block, and there are no other blocks
      }

      // Check if we should unmap this block
      if (enable_unmapping && b->free) {
        size_t total_size = b->size + BLOCK_SIZE;
        printf("Unmapping block at %p, size: %zu\n", (void *)b, total_size);

        // Unmap only when you are sure it is not used elsewhere
        if (munmap(b, total_size) == -1) {
          perror("munmap");
        } else {
          // Reset base if it was pointing to this unmapped block
          if (base == b) {
            base = NULL;
          }
        }
      }
    }

  } else {
    printf("Attempt to free invalid pointer: %p\n", ptr);
  }
  pthread_mutex_unlock(&memory_mutex);
}
