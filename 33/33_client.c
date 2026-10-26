#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 512

static int send_line(int fd, const char *text)
{
    size_t length = strlen(text);
    char buffer[BUFFER_SIZE + 1];
    if (length > BUFFER_SIZE - 1) length = BUFFER_SIZE - 1;
    memcpy(buffer, text, length);
    buffer[length++] = '\n';
    size_t sent = 0;
    while (sent < length) {
        ssize_t count = send(fd, buffer + sent, length - sent, 0);
        if (count == -1) {
            if (errno == EINTR) continue;
            return -1;
        }
        sent += (size_t)count;
    }
    return 0;
}

static int receive_line(int fd, char *buffer, size_t capacity)
{
    size_t used = 0;
    while (used + 1 < capacity) {
        char byte;
        ssize_t count = recv(fd, &byte, 1, 0);
        if (count == 0) return 0;
        if (count == -1) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (byte == '\n') break;
        buffer[used++] = byte;
    }
    buffer[used] = '\0';
    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s SERVER_IP\n", argv[0]);
        return 2;
    }
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) { perror("socket"); return 1; }
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &server.sin_addr) != 1) {
        fprintf(stderr, "Invalid IPv4 address\n"); close(client); return 2;
    }
    if (connect(client, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("connect"); close(client); return 1;
    }
    char message[BUFFER_SIZE], reply[BUFFER_SIZE];
    while (printf("Message (exit to quit): "), fflush(stdout), fgets(message, sizeof(message), stdin)) {
        message[strcspn(message, "\n")] = '\0';
        if (send_line(client, message) == -1) { perror("send"); break; }
        int result = receive_line(client, reply, sizeof(reply));
        if (result <= 0) { if (result == -1) perror("recv"); break; }
        printf("Server: %s\n", reply);
        if (strcmp(message, "exit") == 0) break;
    }
    close(client);
    return 0;
}

/*

nilkanth@zuxxy: gcc 33_client.c -o 33_client.out
nilkanth@zuxxy: ./33_client.out 127.0.0.1
Message (exit to quit): hello
Server: Server received: hello
Message (exit to quit): exit
Server: Goodbye
*/
