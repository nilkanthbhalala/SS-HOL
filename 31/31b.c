#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEM_KEY ((key_t)0x53454d32)

int main(int argc, char **argv)
{
    char *end;
    long initial = argc == 2 ? strtol(argv[1], &end, 10) : 3;
    if ((argc == 2 && (*argv[1] == '\0' || *end != '\0')) || initial < 0 || initial > 32767) {
        fprintf(stderr, "Usage: %s [initial-value 0..32767]\n", argv[0]);
        return 2;
    }
    int id = semget(SEM_KEY, 1, IPC_CREAT | 0660);
    if (id == -1) { perror("semget"); return 1; }
    union semun { int val; struct semid_ds *buf; unsigned short *array; } value;
    value.val = (int)initial;
    if (semctl(id, 0, SETVAL, value) == -1) { perror("semctl"); return 1; }
    printf("Counting semaphore ID: %d, value: %d\n", id, value.val);
    return 0;
}

/*

nilkanth@zuxxy: gcc 31b.c -o a.out
nilkanth@zuxxy: ./a.out
Counting semaphore ID: 5, value: 3
*/
