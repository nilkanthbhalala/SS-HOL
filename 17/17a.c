#define _POSIX_C_SOURCE 200809L
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int channel[2];
    if (pipe(channel) == -1)
        return 1;
    pid_t left = fork();
    if (left == -1)
        return 1;
    if (left == 0) {
        close(channel[0]);
        close(STDOUT_FILENO);
        if (dup(channel[1]) != STDOUT_FILENO)
            _exit(1);
        close(channel[1]);
        execlp("ls", "ls", "-l", (char *)NULL);
        _exit(127);
    }
    pid_t right = fork();
    if (right == -1)
        return 1;
    if (right == 0) {
        close(channel[1]);
        close(STDIN_FILENO);
        if (dup(channel[0]) != STDIN_FILENO)
            _exit(1);
        close(channel[0]);
        execlp("wc", "wc", (char *)NULL);
        _exit(127);
    }
    close(channel[0]);
    close(channel[1]);
    waitpid(left, NULL, 0);
    waitpid(right, NULL, 0);
    return 0;
}

/*

nilkanth@zuxxy: gcc 17a.c -o a.out
nilkanth@zuxxy: ./a.out
      5      38     201
*/
