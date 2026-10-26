#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void *show_id(void *argument)
{
    intptr_t number = (intptr_t)argument;
    printf("Thread %ld ID: %" PRIuPTR "\n", (long)number, (uintptr_t)pthread_self());
    return NULL;
}

int main(void)
{
    pthread_t threads[3];
    for (intptr_t i = 0; i < 3; ++i) {
        int error = pthread_create(&threads[i], NULL, show_id, (void *)i);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            return 1;
        }
    }
    for (int i = 0; i < 3; ++i)
        pthread_join(threads[i], NULL);
    printf("Main thread ID: %" PRIuPTR "\n", (uintptr_t)pthread_self());
    return 0;
}

/*

nilkanth@zuxxy: gcc -pthread 7.c -o a.out
nilkanth@zuxxy: ./a.out
Thread 0 ID: 137843471021760
Thread 1 ID: 137843462629056
Thread 2 ID: 137843317929664
Main thread ID: 137843474925376
*/
