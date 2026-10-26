#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int channel[2];
    if (pipe(channel) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        return 1;
    }
    if (child == 0) {
        char message[128];
        close(channel[1]);
        ssize_t count = read(channel[0], message, sizeof(message) - 1);
        if (count == -1) {
            perror("read");
            return 1;
        }
        message[count] = '\0';
        printf("Child received: %s\n", message);
        close(channel[0]);
        return 0;
    }
    close(channel[0]);
    const char message[] = "Hello from the parent";
    if (write(channel[1], message, sizeof(message)) == -1) {
        perror("write");
        return 1;
    }
    close(channel[1]);
    waitpid(child, NULL, 0);
    return 0;
}

/*

nilkanth@zuxxy: gcc 15.c -o a.out
nilkanth@zuxxy: ./a.out
Child received: Hello from the parent
*/
