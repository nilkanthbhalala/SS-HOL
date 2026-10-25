#define _DEFAULT_SOURCE
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/q32_shared_data"
#define SEM_NAME "/q32_shared_lock"

int main(void)
{
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
    if (fd == -1) { perror("shm_open"); return 1; }
    sem_t *lock = sem_open(SEM_NAME, O_CREAT, 0600, 1);
    if (lock == SEM_FAILED) { perror("sem_open"); close(fd); return 1; }
    if (sem_wait(lock) == -1) { perror("sem_wait"); return 1; }
    struct stat info;
    if (fstat(fd, &info) == -1) { perror("fstat"); return 1; }
    if (info.st_size < (off_t)sizeof(int) && ftruncate(fd, sizeof(int)) == -1) {
        perror("ftruncate"); return 1;
    }
    int *value = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (value == MAP_FAILED) { perror("mmap"); return 1; }
    ++*value;
    printf("Protected shared value: %d\n", *value);
    munmap(value, sizeof(int));
    sem_post(lock);
    sem_close(lock);
    close(fd);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Protected shared value: 1
*/
