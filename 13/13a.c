#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void handle_stop(int signo)
{
    (void)signo;
    puts("SIGSTOP caught");
}

int main(void)
{
    struct sigaction action = {0};
    action.sa_handler = handle_stop;
    sigemptyset(&action.sa_mask);
    printf("Process ID: %ld\n", (long)getpid());
    if (sigaction(SIGSTOP, &action, NULL) == -1)
        printf("SIGSTOP cannot be caught (sigaction: %s)\n", strerror(errno));
    fflush(stdout);
    for (;;)
        pause();
}

/*
nilkanth@zuxxy: ./13a.out
Process ID: 169157
SIGSTOP cannot be caught (sigaction: Invalid argument)
*/
