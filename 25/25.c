#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define QUEUE_KEY ((key_t)0x514c4932)

static void show_time(const char *label, time_t value)
{
    printf("%s: ", label);
    if (value == 0)
        puts("never");
    else
        printf("%s", ctime(&value));
}

int main(void)
{
    int id = msgget(QUEUE_KEY, IPC_CREAT | 0660);
    if (id == -1) {
        perror("msgget");
        return 1;
    }
    struct msqid_ds info;
    if (msgctl(id, IPC_STAT, &info) == -1) {
        perror("msgctl");
        return 1;
    }
    printf("Queue ID: %d\nPermissions: %04o\nUID: %u GID: %u\n",
           id, info.msg_perm.mode & 0777, info.msg_perm.uid, info.msg_perm.gid);
    show_time("Last message sent", info.msg_stime);
    show_time("Last message received", info.msg_rtime);
    show_time("Last queue change", info.msg_ctime);
    printf("Current queue bytes: %lu\nMessages: %lu\nMaximum bytes: %lu\n",
           (unsigned long)info.msg_cbytes, (unsigned long)info.msg_qnum,
           (unsigned long)info.msg_qbytes);
    printf("Last msgsnd PID: %d\nLast msgrcv PID: %d\n", info.msg_lspid, info.msg_lrpid);
    return 0;
}

/*

nilkanth@zuxxy: gcc 25.c -o a.out
nilkanth@zuxxy: ./a.out
Queue ID: 2
Permissions: 0660
UID: 1000 GID: 1000
Last message sent: Sun Oct 25 17:01:04 2026
Last message received: never
Last queue change: Sun Oct 25 17:01:04 2026
Current queue bytes: 35
Messages: 1
Maximum bytes: 16384
Last msgsnd PID: 169279
Last msgrcv PID: 0
*/
