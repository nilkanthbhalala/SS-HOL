#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int ensure_fifo(const char *path)
{
    struct stat info;
    if (mkfifo(path, 0660) == 0 || errno == EEXIST) {
        if (lstat(path, &info) == 0 && S_ISFIFO(info.st_mode))
            return 0;
    }
    perror("mkfifo");
    return -1;
}

int main(void)
{
    if (ensure_fifo("fifo21_ab") || ensure_fifo("fifo21_ba"))
        return 1;
    int incoming = open("fifo21_ab", O_RDONLY);
    if (incoming == -1) { perror("open fifo21_ab"); return 1; }
    int outgoing = open("fifo21_ba", O_WRONLY);
    if (outgoing == -1) { perror("open fifo21_ba"); return 1; }
    char message[128];
    ssize_t count = read(incoming, message, sizeof(message) - 1);
    if (count < 0) { perror("read"); return 1; }
    message[count] = '\0';
    printf("B received: %s\n", message);
    const char reply[] = "Reply from process B";
    if (write(outgoing, reply, sizeof(reply)) == -1) { perror("write"); return 1; }
    close(incoming); close(outgoing);
    return 0;
}

/*
nilkanth@zuxxy: ./21b.out
B received: Message from process A
*/
