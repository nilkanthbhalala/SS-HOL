#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define SEM_NAME "/q32_resource_pool"

int main(void)
{
    sem_t *resources = sem_open(SEM_NAME, O_CREAT, 0600, 2);
    if (resources == SEM_FAILED) { perror("sem_open"); return 1; }
    printf("Process %ld is waiting for one of two resources\n", (long)getpid());
    if (sem_wait(resources) == -1) { perror("sem_wait"); return 1; }
    puts("Resource acquired");
    sleep(3);
    puts("Resource released");
    sem_post(resources);
    sem_close(resources);
    return 0;
}

/*

nilkanth@zuxxy: gcc 32c.c -o a.out
nilkanth@zuxxy: ./a.out
Process 169376 is waiting for one of two resources
Resource acquired
Resource released
*/
