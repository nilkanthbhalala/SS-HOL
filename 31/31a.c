#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEM_KEY ((key_t)0x53454d31)

int main(void)
{
    int id = semget(SEM_KEY, 1, IPC_CREAT | 0660);
    if (id == -1) { perror("semget"); return 1; }
    union semun { int val; struct semid_ds *buf; unsigned short *array; } value;
    value.val = 1;
    if (semctl(id, 0, SETVAL, value) == -1) { perror("semctl"); return 1; }
    printf("Binary semaphore ID: %d, value: %d\n", id, value.val);
    return 0;
}

/*

nilkanth@zuxxy: gcc 31a.c -o a.out
nilkanth@zuxxy: ./a.out
Binary semaphore ID: 4, value: 1
*/
