#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
    struct stat info;
    if (mkfifo("fifo22", 0660) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }
    if (lstat("fifo22", &info) == -1 || !S_ISFIFO(info.st_mode)) {
        fprintf(stderr, "fifo22 is not a FIFO\n");
        return 1;
    }
    int fd = open("fifo22", O_RDWR | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    fd_set readable;
    FD_ZERO(&readable);
    FD_SET(fd, &readable);
    struct timeval timeout = {10, 0};
    int ready = select(fd + 1, &readable, NULL, NULL, &timeout);
    if (ready == -1) {
        perror("select");
        close(fd);
        return 1;
    }
    if (ready == 0) {
        puts("No FIFO data arrived within 10 seconds");
    } else {
        char buffer[256];
        ssize_t count = read(fd, buffer, sizeof(buffer) - 1);
        if (count > 0) {
            buffer[count] = '\0';
            printf("Received: %s\n", buffer);
        } else if (count == -1 && errno != EAGAIN) {
            perror("read");
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Received: data from FIFO
*/
