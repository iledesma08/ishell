#include "../include/memory.h"
#include "../include/calloc.h"
#include "../include/malloc.h"
#include "../include/realloc.h"
#include "../include/free.h"

/*
 * Testing code to test all the functions implemented.
 * First two new variables are created and allocated space using malloc()
 * Then the first varaible is freed and malloc() is called again to show that space can be reused.
 * Then calloc() is used to allocate an int array of size 5. Initially the values will be set to 0 by calloc();
 * Then the array is resized to size 10 using realloc()
 */

int main(void) {
    printf("\n\n");

    printf("Initializing first variable : ");
    int *a = (int*)malloc(sizeof(int));
    if (!a) {
        perror("malloc");
        return 1;
    }
    *a = 5;
    printf("%p -> %d \n\n", (void*)a, *a);

    free(a, FALSE);

    return 0;
}