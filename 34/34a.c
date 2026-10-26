#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 8081
#define BUFFER_SIZE 1024

static void serve_client(int client)
{
    char buffer[BUFFER_SIZE];
    ssize_t count;
    while ((count = read(client, buffer, sizeof(buffer))) > 0) {
        ssize_t sent = 0;
        while (sent < count) {
            ssize_t result = write(client, buffer + sent, (size_t)(count - sent));
            if (result <= 0) return;
            sent += result;
        }
    }
    close(client);
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
    if (bind(server, (struct sockaddr *)&address, sizeof(address)) == -1 || listen(server, 16) == -1) {
        perror("bind/listen"); close(server); return 1;
    }
    signal(SIGCHLD, SIG_IGN);
    printf("Fork server listening on port %d\n", PORT);
    for (;;) {
        int client = accept(server, NULL, NULL);
        if (client == -1) { perror("accept"); continue; }
        pid_t child = fork();
        if (child == -1) { perror("fork"); close(client); continue; }
        if (child == 0) {
            close(server);
            serve_client(client);
            _exit(0);
        }
        close(client);
    }
}

/*

nilkanth@zuxxy: gcc 34a.c -o 34a.out
nilkanth@zuxxy: ./34a.out
Fork server listening on port 8081
nilkanth@zuxxy: printf 'hello from nc\n' | nc -N 127.0.0.1 8081
hello from nc
*/
