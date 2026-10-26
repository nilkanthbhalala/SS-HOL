#include <signal.h>
#include <unistd.h>

static void handle_signal(int signo)
{
    const char message[] = "Caught SIGINT\n";
    (void)signo;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(0);
}

int main(void)
{
    signal(SIGINT, handle_signal);
    raise(SIGINT);
    return 0;
}

/*

nilkanth@zuxxy: gcc 8b.c -o a.out
nilkanth@zuxxy: ./a.out
Caught SIGINT
*/
