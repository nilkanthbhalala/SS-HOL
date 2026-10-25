#include <signal.h>
#include <unistd.h>

static void handle_alarm(int signo)
{
    const char message[] = "Caught SIGALRM from alarm()\n";
    (void)signo;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    _exit(0);
}

int main(void)
{
    signal(SIGALRM, handle_alarm);
    alarm(2);
    for (;;)
        pause();
}

/*
nilkanth@zuxxy: ./a.out
Caught SIGALRM from alarm()
*/
