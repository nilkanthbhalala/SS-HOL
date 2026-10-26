#include <signal.h>
#include <unistd.h>

static void handle_signal(int signo)
{
    const char message[] = "Caught SIGSEGV\n";
    (void)signo;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(0);
}

int main(void)
{
    signal(SIGSEGV, handle_signal);
    raise(SIGSEGV);
    return 0;
}

/*

nilkanth@zuxxy: gcc 8a.c -o a.out
nilkanth@zuxxy: ./a.out
Caught SIGSEGV
*/
