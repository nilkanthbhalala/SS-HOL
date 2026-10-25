#define _DEFAULT_SOURCE
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

static void show_limit(const char *label, int name)
{
    errno = 0;
    long value = sysconf(name);
    if (value == -1 && errno != 0)
        printf("%s: unavailable\n", label);
    else
        printf("%s: %ld\n", label, value);
}

int main(void)
{
    show_limit("Maximum exec argument length", _SC_ARG_MAX);
    show_limit("Maximum child processes per user", _SC_CHILD_MAX);
    show_limit("Clock ticks per second", _SC_CLK_TCK);
    show_limit("Maximum open files", _SC_OPEN_MAX);
    show_limit("Page size in bytes", _SC_PAGESIZE);
#ifdef _SC_PHYS_PAGES
    show_limit("Physical memory pages", _SC_PHYS_PAGES);
#endif
#ifdef _SC_AVPHYS_PAGES
    show_limit("Available physical pages", _SC_AVPHYS_PAGES);
#endif
    return 0;
}

/*
nilkanth@zuxxy: ./a.out
Maximum exec argument length: 2097152
Maximum child processes per user: 48691
Clock ticks per second: 100
Maximum open files: 524288
Page size in bytes: 4096
Physical memory pages: 3791418
Available physical pages: 493788
*/
