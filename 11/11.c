#include <signal.h>
#include <stdio.h>

int main(void)
{
    struct sigaction action = {0};
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGINT, &action, NULL) == -1)
        return 1;
    puts("SIGINT is ignored");
    raise(SIGINT);
    action.sa_handler = SIG_DFL;
    if (sigaction(SIGINT, &action, NULL) == -1)
        return 1;
    puts("Default SIGINT action restored; raising SIGINT now");
    fflush(stdout);
    raise(SIGINT);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
SIGINT is ignored
Default SIGINT action restored; raising SIGINT now
*/
