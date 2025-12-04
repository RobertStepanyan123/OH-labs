#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static void * threadFunc(void *arg)
{
    char *s = (char *) arg;
    printf("%s\n", s);

    size_t length = strlen(s);
    size_t *lengthP = malloc(sizeof(size_t));
    *lengthP = length;

    return lengthP;
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    void *res;
    int s;

    s = pthread_create(&t1, NULL, threadFunc, "Hello world");
    if (s != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(EXIT_FAILURE);
    }

    printf("Message from main()\n");

    s = pthread_join(t1, &res);
    if (s != 0) {
        fprintf(stderr, "pthread_join error\n");
        exit(EXIT_FAILURE);
    }

    printf("Thread returned %zu\n", *(size_t*)res);
    free(res);

    return 0;
}