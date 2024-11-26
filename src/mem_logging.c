#include "mem_logging.h"

extern size_t total_allocated_memory; // Total allocated memory
extern size_t total_freed_memory;     // Total freed memory

t_log_entry *log_head = NULL; // Head of the log list

void log_mem_operation(alloc_type type, void *ptr, size_t size, unsigned long *op_ctr) {
  t_log_entry *entry = mmap(NULL, sizeof(t_log_entry), PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (entry == MAP_FAILED) {
    // Use write or fprintf to stderr to avoid using printf
    fprintf(stderr, "Failed to allocate memory for log entry.\n");
    return;
  }
  entry->type = type;
  entry->ptr = ptr;
  entry->size = size;
  entry->total_allocated = total_allocated_memory;
  entry->total_freed = total_freed_memory;
  entry->next = log_head;
  entry->op_id = *op_ctr;
  (*op_ctr)++;
  log_head = entry;
}

void clear_logs(void) {
  t_log_entry *entry = log_head;
  while (entry) {
    t_log_entry *next = entry->next;
    munmap(entry, sizeof(t_log_entry));
    entry = next;
  }
  log_head = NULL;
}
