#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int channel[2];
    char received[128];
    const char message[] = "Data sent through a pipe";
    if (pipe(channel) == -1) {
        perror("pipe");
        return 1;
    }
    if (write(channel[1], message, sizeof(message)) == -1) {
        perror("write");
        return 1;
    }
    close(channel[1]);
    ssize_t length = read(channel[0], received, sizeof(received) - 1);
    if (length == -1) {
        perror("read");
        return 1;
    }
    received[length] = '\0';
    printf("Read from pipe: %s\n", received);
    close(channel[0]);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Read from pipe: Data sent through a pipe
*/
