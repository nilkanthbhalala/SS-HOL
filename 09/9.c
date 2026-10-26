#include <signal.h>
#include <stdio.h>

int main(void)
{
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        return 1;
    puts("SIGINT is ignored");
    raise(SIGINT);
    if (signal(SIGINT, SIG_DFL) == SIG_ERR)
        return 1;
    puts("Default SIGINT action restored; raising SIGINT now");
    fflush(stdout);
    raise(SIGINT);
    return 0;
}

/*

nilkanth@zuxxy: gcc 9.c -o a.out
nilkanth@zuxxy: ./a.out
SIGINT is ignored
Default SIGINT action restored; raising SIGINT now
*/
