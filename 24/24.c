#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY ((key_t)0x514c4932)

int main(void)
{
    int id = msgget(QUEUE_KEY, IPC_CREAT | 0660);
    if (id == -1) {
        perror("msgget");
        return 1;
    }
    printf("Key: 0x%lx\nQueue ID: %d\n", (unsigned long)QUEUE_KEY, id);
    return 0;
}

/*

nilkanth@zuxxy: gcc 24.c -o a.out
nilkanth@zuxxy: ./a.out
Key: 0x514c4932
Queue ID: 2
*/
