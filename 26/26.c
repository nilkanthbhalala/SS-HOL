#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY ((key_t)0x514c4932)

struct message { long type; char text[128]; };

int main(void)
{
    int id = msgget(QUEUE_KEY, IPC_CREAT | 0660);
    if (id == -1) {
        perror("msgget");
        return 1;
    }
    struct message message = {1, "Message sent to the System V queue"};
    if (msgsnd(id, &message, strlen(message.text) + 1, 0) == -1) {
        perror("msgsnd");
        return 1;
    }
    printf("Sent: %s\nQueue ID: %d\n", message.text, id);
    return 0;
}

/*

nilkanth@zuxxy: gcc 26.c -o a.out
nilkanth@zuxxy: ./a.out
Sent: Message sent to the System V queue
Queue ID: 2
*/
