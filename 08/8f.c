#define _DEFAULT_SOURCE
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

static volatile sig_atomic_t count;

static void handle_alarm(int signo)
{
    const char message[] = "Caught SIGVTALRM from ITIMER_VIRTUAL\n";
    (void)signo;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    if (++count == 3)
        _exit(0);
}

int main(void)
{
    struct itimerval timer = {{1, 0}, {1, 0}};
    if (signal(SIGVTALRM, handle_alarm) == SIG_ERR || setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1)
        return 1;
    for (;;)
        ;
}

/*
nilkanth@zuxxy: ./a.out
Caught SIGVTALRM from ITIMER_VIRTUAL
Caught SIGVTALRM from ITIMER_VIRTUAL
Caught SIGVTALRM from ITIMER_VIRTUAL
*/
