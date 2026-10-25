#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

int main(void)
{
    struct rlimit limit;
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }
    printf("Maximum open files per process: %llu\n", (unsigned long long)limit.rlim_cur);
    int channel[2];
    if (pipe(channel) == -1) {
        perror("pipe");
        return 1;
    }
#ifdef F_GETPIPE_SZ
    int size = fcntl(channel[0], F_GETPIPE_SZ);
    if (size == -1)
        perror("F_GETPIPE_SZ");
    else
        printf("Pipe capacity: %d bytes\n", size);
#else
    puts("Pipe capacity query is unavailable on this system");
#endif
    close(channel[0]);
    close(channel[1]);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Maximum open files per process: 524288
Pipe capacity: 65536 bytes
*/
