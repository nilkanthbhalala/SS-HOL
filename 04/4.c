#define _POSIX_C_SOURCE 200809L
#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static uint64_t timestamp(void)
{
#if defined(__i386__) || defined(__x86_64__)
    unsigned int low, high;
    __asm__ volatile("lfence; rdtsc" : "=a"(low), "=d"(high) :: "memory");
    return ((uint64_t)high << 32) | low;
#elif defined(__aarch64__)
    uint64_t value;
    __asm__ volatile("isb; mrs %0, cntvct_el0" : "=r"(value));
    return value;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
#endif
}

int main(void)
{
    uint64_t start = timestamp();
    for (int i = 0; i < 100; ++i)
        (void)getppid();
    uint64_t elapsed = timestamp() - start;
#if defined(__i386__) || defined(__x86_64__)
    puts("Timestamp counter cycles for 100 getppid calls:");
#else
    puts("Timestamp ticks for 100 getppid calls:");
#endif
    printf("Total: %" PRIu64 "\nAverage: %" PRIu64 "\n", elapsed, elapsed / 100);
    return 0;
}

/*

nilkanth@zuxxy: gcc 4.c -o a.out
nilkanth@zuxxy: ./a.out
Timestamp counter cycles for 100 getppid calls:
Total: 95726
Average: 957
*/
