#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int id = shmget(IPC_PRIVATE, 256, IPC_CREAT | 0600);
    if (id == -1) { perror("shmget"); return 1; }
    char *memory = shmat(id, NULL, 0);
    if (memory == (void *)-1) { perror("shmat"); shmctl(id, IPC_RMID, NULL); return 1; }
    strcpy(memory, "Shared memory data");
    printf("Written: %s\n", memory);
    char *read_only = shmat(id, NULL, SHM_RDONLY);
    if (read_only == (void *)-1) {
        perror("shmat read-only");
        shmdt(memory); shmctl(id, IPC_RMID, NULL); return 1;
    }
    printf("Read-only attachment: %s\n", read_only);
    fflush(stdout);
    pid_t child = fork();
    if (child == -1) { perror("fork"); return 1; }
    if (child == 0) {
        struct rlimit no_core = {0, 0};
        setrlimit(RLIMIT_CORE, &no_core);
        volatile char *view = read_only;
        view[0] = 'X';
        _exit(0);
    }
    int status;
    waitpid(child, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV)
        puts("Read-only attachment prevented writing (SIGSEGV)");
    else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        puts("Write unexpectedly succeeded");
    shmdt(read_only);
    shmdt(memory);
    if (shmctl(id, IPC_RMID, NULL) == -1) { perror("IPC_RMID"); return 1; }
    puts("Detached and removed shared memory");
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Written: Shared memory data
Read-only attachment: Shared memory data
Read-only attachment prevented writing (SIGSEGV)
Detached and removed shared memory
*/
