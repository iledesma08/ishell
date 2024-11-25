#include "include/malloc.h"

extern base;
extern method;

// Function to find a suitable memory block based on the allocation method
t_block find_block(t_block* last, size_t size)
{
    t_block b = base;

    if (method == FIRST_FIT)
    {
        while (b && !(b->free && b->size >= size))
        {
            *last = b;
            b = b->next;
        }

        return (b);
    }
    else
    {
        size_t dif = PAGESIZE;
        t_block best = NULL;

        while (b)
        {
            if (b->free)
            {
                if (b->size == size)
                {
                    return b;
                }
                if (b->size > size && (b->size - size) < dif)
                {
                    dif = b->size - size;
                    best = b;
                }
            }
            *last = b;
            b = b->next;
        }
        return best;
    }
}

// Function to split a memory block into two
void split_block(t_block b, size_t s)
{
    if (b->size <= s + BLOCK_SIZE)
    {
        return;
    }

    t_block new;
    new = (t_block)(b->data + s);
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->free = 1;
    b->size = s;
    b->next = new;
}

// Function to extend the heap by creating a new block
t_block extend_heap(t_block last, size_t s)
{
    t_block b;
    b = mmap(0, s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (b == MAP_FAILED)
    {
        return NULL;
    }
    b->size = s;
    b->next = NULL;
    b->prev = last;
    b->ptr = b->data;

    if (last)
        last->next = b;

    b->free = 0;
    return b;
}

// Function to allocate memory
void* malloc(size_t size)
{
    t_block b, last;
    size_t s;
    s = align(size);

    if (base)
    {
        last = base;
        b = find_block(&last, s);
        if (b)
        {
            if ((b->size - s) >= (BLOCK_SIZE + 4))
                split_block(b, s);
            b->free = 0;
        }
        else
        {
            b = extend_heap(last, s);
            if (!b)
                return (NULL);
        }
    }
    else
    {
        b = extend_heap(NULL, s);
        if (!b)
            return (NULL);
        base = b;
    }
    return (b->data);
}
