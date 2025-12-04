#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* thread1_func(void* arg) {
    int num = *(int*)arg;
    int* result = malloc(sizeof(int));
    *result = num * 2;
    return result;
}

void* thread2_func(void* arg) {
    int num = *(int*)arg;
    int* result = malloc(sizeof(int));
    *result = num * 3;
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Using: %s <number>\n", argv[0]);
        return 1;
    }

    int num = atoi(argv[1]);
    pthread_t t1, t2;
    int* result1;
    int* result2;

    pthread_create(&t1, NULL, thread1_func, &num);
    pthread_join(t1, (void**)&result1);  

    pthread_create(&t2, NULL, thread2_func, result1);
    pthread_join(t2, (void**)&result2);

    printf("Thread returned = %d\n", *result2);

    free(result1);
    free(result2);

    return 0;
}