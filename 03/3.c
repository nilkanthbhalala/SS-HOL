#include <stdio.h>
#include <sys/resource.h>

int main(void)
{
    struct rlimit limit;
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }
    printf("Open-file limit before: soft=%llu hard=%llu\n",
           (unsigned long long)limit.rlim_cur, (unsigned long long)limit.rlim_max);
    if (limit.rlim_cur > 256)
        limit.rlim_cur = limit.rlim_max < 256 ? limit.rlim_max : 256;
    if (setrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("setrlimit");
        return 1;
    }
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }
    printf("Open-file limit after:  soft=%llu hard=%llu\n",
           (unsigned long long)limit.rlim_cur, (unsigned long long)limit.rlim_max);
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Open-file limit before: soft=524288 hard=524288
Open-file limit after:  soft=256 hard=524288
*/
