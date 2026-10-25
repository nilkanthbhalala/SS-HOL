#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void *worker(void *argument)
{
    intptr_t number = (intptr_t)argument;
    printf("Thread %ld is running\n", (long)number);
    return NULL;
}

int main(void)
{
    pthread_t threads[3];
    for (intptr_t i = 0; i < 3; ++i) {
        int error = pthread_create(&threads[i], NULL, worker, (void *)i);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            return 1;
        }
    }
    for (int i = 0; i < 3; ++i) {
        int error = pthread_join(threads[i], NULL);
        if (error != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(error));
            return 1;
        }
    }
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Thread 0 is running
Thread 1 is running
Thread 2 is running
*/
