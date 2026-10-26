#include <signal.h>
#include <unistd.h>

static void handle_signal(int signo)
{
    const char message[] = "Caught SIGFPE with sigaction\n";
    (void)signo;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(0);
}

int main(void)
{
    struct sigaction action = {0};
    action.sa_handler = handle_signal;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGFPE, &action, NULL) == -1)
        return 1;
    raise(SIGFPE);
    return 0;
}

/*

nilkanth@zuxxy: gcc 10c.c -o a.out
nilkanth@zuxxy: ./a.out
Caught SIGFPE with sigaction
*/
