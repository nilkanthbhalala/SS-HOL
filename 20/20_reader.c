#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

static int ensure_fifo(void)
{
    struct stat info;
    if (mkfifo("fifo20", 0660) == 0 || errno == EEXIST) {
        if (lstat("fifo20", &info) == 0 && S_ISFIFO(info.st_mode))
            return 0;
    }
    perror("mkfifo");
    return -1;
}

int main(void)
{
    if (ensure_fifo() == -1)
        return 1;
    int fd = open("fifo20", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    char buffer[256];
    ssize_t count = read(fd, buffer, sizeof(buffer) - 1);
    if (count == -1) {
        perror("read");
        close(fd);
        return 1;
    }
    buffer[count] = '\0';
    printf("Received: %s\n", buffer);
    close(fd);
    return 0;
}

/*

nilkanth@zuxxy: gcc 20_reader.c -o 20_reader.out
nilkanth@zuxxy: ./20_reader.out
Received: One-way FIFO message
*/
