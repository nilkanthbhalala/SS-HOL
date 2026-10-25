#include <errno.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY ((key_t)0x514c4932)

struct message { long type; char text[257]; };

int main(void)
{
    int id = msgget(QUEUE_KEY, IPC_CREAT | 0660);
    if (id == -1) { perror("msgget"); return 1; }
    struct message message;
    ssize_t length = msgrcv(id, &message, sizeof(message.text) - 1, 0, IPC_NOWAIT);
    if (length == -1) {
        if (errno == ENOMSG) {
            puts("No message is currently available");
            return 0;
        }
        perror("msgrcv");
        return 1;
    }
    message.text[length] = '\0';
    printf("Received: %s\n", message.text);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
No message is currently available
*/
