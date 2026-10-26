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
    int fd = open("fifo20", O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    const char message[] = "One-way FIFO message";
    if (write(fd, message, sizeof(message)) == -1) {
        perror("write");
        close(fd);
        return 1;
    }
    printf("Sent: %s\n", message);
    close(fd);
    return 0;
}

/*

nilkanth@zuxxy: gcc 20_writer.c -o 20_writer.out
nilkanth@zuxxy: ./20_writer.out
Sent: One-way FIFO message
*/
