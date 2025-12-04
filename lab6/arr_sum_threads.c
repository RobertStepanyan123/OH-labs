#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int data[SIZE] = {1,2,3,4,5,6,7,8,9,10}; 

void* sum_first_half(void* arg) {
    int* sum = malloc(sizeof(int));
    *sum = 0;
    for (int i = 0; i < SIZE/2; i++) {
        *sum += data[i];
    }
    return sum;
}

void* sum_second_half(void* arg) {
    int* sum = malloc(sizeof(int));
    *sum = 0;
    for (int i = SIZE/2; i < SIZE; i++) {
        *sum += data[i];
    }
    return sum;
}

int main() {
    pthread_t t1, t2;
    int* sum1;
    int* sum2;

    pthread_create(&t1, NULL, sum_first_half, NULL);
    pthread_create(&t2, NULL, sum_second_half, NULL);

    pthread_join(t1, (void**)&sum1);
    pthread_join(t2, (void**)&sum2);

    int total_sum = *sum1 + *sum2;

    printf("first half sum = %d\n", *sum1);
    printf("second half sum = %d\n", *sum2);
    printf("sum = %d\n", total_sum);

    free(sum1);
    free(sum2);

    return 0;
}
