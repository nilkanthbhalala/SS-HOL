#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>

int main(void)
{
    const char *names[] = {"/q32_ticket_lock", "/q32_shared_lock", "/q32_resource_pool"};
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); ++i) {
        if (sem_unlink(names[i]) == 0)
            printf("Removed semaphore %s\n", names[i]);
        else if (errno != ENOENT)
            perror(names[i]);
    }
    if (shm_unlink("/q32_shared_data") == 0)
        puts("Removed shared memory object /q32_shared_data");
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Removed semaphore /q32_ticket_lock
Removed semaphore /q32_shared_lock
Removed semaphore /q32_resource_pool
Removed shared memory object /q32_shared_data
*/
