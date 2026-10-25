#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY ((key_t)0x514c4932)

int main(void)
{
    int id = msgget(QUEUE_KEY, 0660);
    if (id == -1) { perror("msgget"); return 1; }
    if (msgctl(id, IPC_RMID, NULL) == -1) { perror("IPC_RMID"); return 1; }
    printf("Removed message queue %d\n", id);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Removed message queue 2
*/
