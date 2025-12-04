#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int data[SIZE];  

void* fill_even(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        if (i % 2 == 1) { 
            data[i] = i + 1;
        }
    }
    return NULL;
}

int main() {
    pthread_t t_even;
    pthread_create(&t_even, NULL, fill_even, NULL);

    for (int i = 0; i < SIZE; i++) {
        if (i % 2 == 0) { 
            data[i] = i + 1;
        }
    }

    pthread_join(t_even, NULL);
    printf("array:\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", data[i]);
    }

    printf("\n");
    return 0;
}