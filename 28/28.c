#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY ((key_t)0x514c4932)

int main(void)
{
    int id = msgget(QUEUE_KEY, IPC_CREAT | 0660);
    if (id == -1) { perror("msgget"); return 1; }
    struct msqid_ds info;
    if (msgctl(id, IPC_STAT, &info) == -1) { perror("IPC_STAT"); return 1; }
    info.msg_perm.mode = (info.msg_perm.mode & ~0777) | 0600;
    if (msgctl(id, IPC_SET, &info) == -1) { perror("IPC_SET"); return 1; }
    printf("Queue %d permissions changed to 0600\n", id);
    return 0;
}

/*

nilkanth@zuxxy: gcc 28.c -o a.out
nilkanth@zuxxy: ./a.out
Queue 2 permissions changed to 0600
*/
