#include <signal.h>
#include <unistd.h>

static void handle_signal(int signo)
{
    const char message[] = "Caught SIGSEGV with sigaction\n";
    (void)signo;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(0);
}

int main(void)
{
    struct sigaction action = {0};
    action.sa_handler = handle_signal;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGSEGV, &action, NULL) == -1)
        return 1;
    raise(SIGSEGV);
    return 0;
}

/*

nilkanth@zuxxy: gcc 10a.c -o a.out
nilkanth@zuxxy: ./a.out
Caught SIGSEGV with sigaction
*/
