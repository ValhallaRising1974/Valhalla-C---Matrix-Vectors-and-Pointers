#include <stdio.h>
#include "headers.h"

void demonstrate_pointers() {
    int value = 10;
    int *ptr = &value;
    printf("Pointer demonstration:\n");
    printf("Value: %d, Address: %p, Dereferenced: %d\n\n", value, ptr, *ptr);
}

void demonstrate_vectors() {
    int vector[5] = {1, 2, 3, 4, 5};
    printf("Vector demonstration:\n");
    for(int i = 0; i < 5; i++) {
        printf("vector[%d] = %d\n", i, vector[i]);
    }
    printf("\n");
}

void demonstrate_matrices() {
    int matrix[2][2] = {{1, 2}, {3, 4}};
    printf("Matrix demonstration:\n");
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            printf("matrix[%d][%d] = %d\t", i, j, matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}