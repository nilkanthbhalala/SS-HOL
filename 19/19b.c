#include <stdio.h>
#include <sys/stat.h>

int main(void)
{
    const char *path = "fifo_mkfifo";
    if (mkfifo(path, 0660) == -1) {
        perror("mkfifo");
        return 1;
    }
    printf("Created FIFO: %s\n", path);
    return 0;
}

/*

nilkanth@zuxxy: gcc 19b.c -o a.out
nilkanth@zuxxy: ./a.out
Created FIFO: fifo_mkfifo
*/
