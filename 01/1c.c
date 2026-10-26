#define _DEFAULT_SOURCE
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

static void handle_timer(int signo)
{
    const char message[] = "ITIMER_PROF expired\n";
    (void)signo;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
}

int main(void)
{
    struct sigaction action = {0};
    struct itimerval timer = {{10, 10}, {10, 0}};
    action.sa_handler = handle_timer;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGPROF, &action, NULL) == -1 || setitimer(ITIMER_PROF, &timer, NULL) == -1)
        return 1;
    for (;;)
        ;
}

/*

nilkanth@zuxxy: gcc 1c.c -o a.out
nilkanth@zuxxy: ./a.out
ITIMER_PROF expired
ITIMER_PROF expired
*/
