#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int first[2], second[2];
    if (pipe(first) == -1 || pipe(second) == -1)
        return 1;
    pid_t listing = fork();
    if (listing == -1)
        return 1;
    if (listing == 0) {
        dup2(first[1], STDOUT_FILENO);
        close(first[0]); close(first[1]); close(second[0]); close(second[1]);
        execlp("ls", "ls", "-l", (char *)NULL);
        _exit(127);
    }
    pid_t filtering = fork();
    if (filtering == -1)
        return 1;
    if (filtering == 0) {
        dup2(first[0], STDIN_FILENO);
        dup2(second[1], STDOUT_FILENO);
        close(first[0]); close(first[1]); close(second[0]); close(second[1]);
        execlp("grep", "grep", "^d", (char *)NULL);
        _exit(127);
    }
    pid_t counting = fork();
    if (counting == -1)
        return 1;
    if (counting == 0) {
        dup2(second[0], STDIN_FILENO);
        close(first[0]); close(first[1]); close(second[0]); close(second[1]);
        execlp("wc", "wc", "-l", (char *)NULL);
        _exit(127);
    }
    close(first[0]); close(first[1]); close(second[0]); close(second[1]);
    waitpid(listing, NULL, 0);
    waitpid(filtering, NULL, 0);
    waitpid(counting, NULL, 0);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
0
*/
