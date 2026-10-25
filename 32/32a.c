#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define SEM_NAME "/q32_ticket_lock"
#define DATA_FILE "ticket.dat"

int main(void)
{
    sem_t *lock = sem_open(SEM_NAME, O_CREAT, 0600, 1);
    if (lock == SEM_FAILED) { perror("sem_open"); return 1; }
    if (sem_wait(lock) == -1) { perror("sem_wait"); return 1; }
    int fd = open(DATA_FILE, O_RDWR | O_CREAT, 0600);
    if (fd == -1) { perror("open"); sem_post(lock); return 1; }
    struct stat info;
    int ticket = 0;
    if (fstat(fd, &info) == -1) { perror("fstat"); close(fd); sem_post(lock); return 1; }
    if (info.st_size >= (off_t)sizeof(ticket)) {
        if (pread(fd, &ticket, sizeof(ticket), 0) != sizeof(ticket)) {
            perror("pread"); close(fd); sem_post(lock); return 1;
        }
    }
    printf("Current ticket: %d\n", ticket);
    ++ticket;
    if (pwrite(fd, &ticket, sizeof(ticket), 0) != sizeof(ticket)) {
        perror("pwrite"); close(fd); sem_post(lock); return 1;
    }
    printf("Issued ticket: %d\n", ticket);
    close(fd);
    sem_post(lock);
    sem_close(lock);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Current ticket: 0
Issued ticket: 1
*/
