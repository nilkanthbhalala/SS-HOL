#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        return 1;
    }
    if (child == 0) {
        printf("Child %ld is sending SIGKILL to parent %ld\n", (long)getpid(), (long)getppid());
        fflush(stdout);
        if (kill(getppid(), SIGKILL) == -1) {
            perror("kill");
            return 1;
        }
        sleep(1);
        printf("Child is now orphaned; new parent is %ld\n", (long)getppid());
        return 0;
    }
    for (;;)
        pause();
}

/*
nilkanth@zuxxy: ./a.out
Child 169144 is sending SIGKILL to parent 169143
Child is now orphaned; new parent is 4360
*/
