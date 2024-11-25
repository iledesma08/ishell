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

    printf("Initializing second variable : ");
    int *b = (int*)malloc(sizeof(int));
    if (!b) {
        perror("malloc");
        free(a);
        return 1;
    }
    *b = 10;
    printf("%p -> %d \n\n", (void*)b, *b);

    free(a);
    printf("Freeing first variable and using that memory block again using malloc() for third variable: ");
    int *c = (int*)malloc(sizeof(int));
    if (!c) {
        perror("malloc");
        free(b);
        return 1;
    }
    *c = 0; // Initialize the value to avoid uninitialized read
    printf("%p -> %d \n\n", (void*)c, *c);

    printf("Initializing int array of size 5 using calloc() : ");
    int *arr = (int*)calloc(5, sizeof(int));
    if (!arr) {
        perror("calloc");
        free(b);
        free(c);
        return 1;
    }
    printf("%p ->  ", (void*)arr);
    for (int i = 0; i < 5; i++) {
        printf(" %d ", arr[i]);
    }
    printf("\n\n");

    printf("Resizing it to size 10 using realloc(): ");
    int *new_arr = (int*)realloc(arr, 10 * sizeof(int));
    if (!new_arr) {
        perror("realloc");
        free(b);
        free(c);
        free(arr);
        return 1;
    }
    arr = new_arr;
    // Initialize the new elements to avoid uninitialized read
    for (int i = 5; i < 10; i++) {
        arr[i] = 0;
    }
    printf("%p ->  ", (void*)arr);
    for (int i = 0; i < 10; i++) {
        printf(" %d ", arr[i]);
    }
    printf("\n");

    free(c);
    free(b);
    free(arr);

    return 0;
}