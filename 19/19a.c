#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
    const char *path = "fifo_mknod";
    if (mknod(path, S_IFIFO | 0660, 0) == -1) {
        perror("mknod");
        return 1;
    }
    printf("Created FIFO: %s\n", path);
    return 0;
}

/*

nilkanth@zuxxy: gcc 19a.c -o a.out
nilkanth@zuxxy: ./a.out
Created FIFO: fifo_mknod
*/
