#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8082
#define BUFFER_SIZE 1024

static void *serve_client(void *argument)
{
    int client = *(int *)argument;
    free(argument);
    char buffer[BUFFER_SIZE];
    ssize_t count;
    while ((count = read(client, buffer, sizeof(buffer))) > 0) {
        ssize_t sent = 0;
        while (sent < count) {
            ssize_t result = write(client, buffer + sent, (size_t)(count - sent));
            if (result <= 0) { close(client); return NULL; }
            sent += result;
        }
    }
    close(client);
    return NULL;
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
    printf("Pthread server listening on port %d\n", PORT);
    for (;;) {
        int client = accept(server, NULL, NULL);
        if (client == -1) { perror("accept"); continue; }
        int *argument = malloc(sizeof(*argument));
        if (argument == NULL) { perror("malloc"); close(client); continue; }
        *argument = client;
        pthread_t thread;
        int error = pthread_create(&thread, NULL, serve_client, argument);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            free(argument);
            close(client);
            continue;
        }
        pthread_detach(thread);
    }
}

/*
nilkanth@zuxxy: ./34b.out
Pthread server listening on port 8082
nilkanth@zuxxy: printf 'hello from nc\n' | nc -N 127.0.0.1 8082
hello from nc
*/
