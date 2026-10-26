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

int main(void)
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) { perror("socket"); return 1; }
    int reuse = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    if (bind(server, (struct sockaddr *)&address, sizeof(address)) == -1 || listen(server, 5) == -1) {
        perror("bind/listen"); close(server); return 1;
    }
    printf("Listening on port %d\n", PORT);
    int client = accept(server, NULL, NULL);
    if (client == -1) { perror("accept"); close(server); return 1; }
    char request[BUFFER_SIZE], reply[BUFFER_SIZE];
    while (receive_line(client, request, sizeof(request)) > 0) {
        printf("Client: %s\n", request);
        if (strcmp(request, "exit") == 0) {
            send_line(client, "Goodbye");
            break;
        }
        snprintf(reply, sizeof(reply), "Server received: %.480s", request);
        if (send_line(client, reply) == -1) { perror("send"); break; }
    }
    close(client);
    close(server);
    return 0;
}

/*

nilkanth@zuxxy: gcc 33_server.c -o 33_server.out
nilkanth@zuxxy: ./33_server.out
Listening on port 8080
Client: hello
Client: exit
*/
