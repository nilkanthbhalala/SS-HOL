#include <stdio.h>
#include <sys/resource.h>

struct resource_entry { int id; const char *name; };

static void show_limit(const struct resource_entry *entry)
{
    struct rlimit limit;
    if (getrlimit(entry->id, &limit) == -1) {
        perror(entry->name);
        return;
    }
    printf("%-24s soft=", entry->name);
    if (limit.rlim_cur == RLIM_INFINITY) printf("unlimited");
    else printf("%llu", (unsigned long long)limit.rlim_cur);
    printf(" hard=");
    if (limit.rlim_max == RLIM_INFINITY) printf("unlimited\n");
    else printf("%llu\n", (unsigned long long)limit.rlim_max);
}

int main(void)
{
    const struct resource_entry resources[] = {
        {RLIMIT_CPU, "CPU time"}, {RLIMIT_FSIZE, "File size"},
        {RLIMIT_DATA, "Data segment"}, {RLIMIT_STACK, "Stack size"},
        {RLIMIT_CORE, "Core file size"}, {RLIMIT_NOFILE, "Open files"},
#ifdef RLIMIT_NPROC
        {RLIMIT_NPROC, "Processes per user"},
#endif
#ifdef RLIMIT_MEMLOCK
        {RLIMIT_MEMLOCK, "Locked memory"},
#endif
#ifdef RLIMIT_AS
        {RLIMIT_AS, "Address space"},
#endif
    };
    for (size_t i = 0; i < sizeof(resources) / sizeof(resources[0]); ++i)
        show_limit(&resources[i]);
    return 0;
}

/*

nilkanth@zuxxy: gcc 2.c -o a.out
nilkanth@zuxxy: ./a.out
CPU time                 soft=unlimited hard=unlimited
File size                soft=unlimited hard=unlimited
Data segment             soft=unlimited hard=unlimited
Stack size               soft=8388608 hard=unlimited
Core file size           soft=0 hard=unlimited
Open files               soft=524288 hard=524288
Processes per user       soft=48691 hard=48691
Locked memory            soft=8388608 hard=8388608
Address space            soft=unlimited hard=unlimited
*/
