#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int parent_to_child[2], child_to_parent[2];
    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        return 1;
    }
    if (child == 0) {
        char incoming[128];
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        ssize_t count = read(parent_to_child[0], incoming, sizeof(incoming) - 1);
        if (count < 0) {
            perror("read");
            return 1;
        }
        incoming[count] = '\0';
        printf("Child received: %s\n", incoming);
        const char reply[] = "Reply from child";
        if (write(child_to_parent[1], reply, sizeof(reply)) == -1) {
            perror("write");
            return 1;
        }
        close(parent_to_child[0]);
        close(child_to_parent[1]);
        return 0;
    }
    close(parent_to_child[0]);
    close(child_to_parent[1]);
    const char message[] = "Message from parent";
    if (write(parent_to_child[1], message, sizeof(message)) == -1) {
        perror("write");
        return 1;
    }
    char reply[128];
    ssize_t count = read(child_to_parent[0], reply, sizeof(reply) - 1);
    if (count < 0) {
        perror("read");
        return 1;
    }
    reply[count] = '\0';
    printf("Parent received: %s\n", reply);
    close(parent_to_child[1]);
    close(child_to_parent[0]);
    waitpid(child, NULL, 0);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Child received: Message from parent
Parent received: Reply from child
*/
