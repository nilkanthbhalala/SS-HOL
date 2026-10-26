#include <signal.h>
#include <unistd.h>

static void handle_signal(int signo)
{
    const char message[] = "Caught SIGFPE\n";
    (void)signo;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(0);
}

int main(void)
{
    signal(SIGFPE, handle_signal);
    raise(SIGFPE);
    return 0;
}

/*

nilkanth@zuxxy: gcc 8c.c -o a.out
nilkanth@zuxxy: ./a.out
Caught SIGFPE
*/
