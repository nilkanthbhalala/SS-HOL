#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s PID\n", argv[0]);
        return 2;
    }
    char *end;
    long value = strtol(argv[1], &end, 10);
    if (*argv[1] == '\0' || *end != '\0' || value <= 1) {
        fprintf(stderr, "Invalid PID\n");
        return 2;
    }
    if (kill((pid_t)value, SIGSTOP) == -1) {
        perror("kill");
        return 1;
    }
    printf("Sent SIGSTOP to process %ld\n", value);
    return 0;
}

/*

nilkanth@zuxxy: gcc 13b.c -o 13b.out
nilkanth@zuxxy: ./13b.out 169157
Sent SIGSTOP to process 169157
*/
