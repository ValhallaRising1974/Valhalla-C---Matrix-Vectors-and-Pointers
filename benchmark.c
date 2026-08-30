#define _POSIX_C_SOURCE 200809L

#include "rle_fill.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef void (*fill_fn)(uint8_t *, uint8_t, size_t);

static uint64_t now_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

static uint64_t checksum(const uint8_t *p, size_t n)
{
    uint64_t h = 1469598103934665603ull;
    for (size_t i = 0; i < n; ++i) {
        h ^= p[i];
        h *= 1099511628211ull;
    }
    return h;
}

static double bench(fill_fn fn, uint8_t *buf, size_t size,
                    size_t iterations, uint8_t seed, uint64_t *out_checksum)
{
    for (size_t i = 0; i < 32; ++i)
        fn(buf, (uint8_t)(seed + i), size);

    uint64_t t0 = now_ns();

    for (size_t i = 0; i < iterations; ++i)
        fn(buf, (uint8_t)(seed + i), size);

    uint64_t t1 = now_ns();

    *out_checksum = checksum(buf, size);
    return (double)(t1 - t0) / 1.0e9;
}

static void print_result(const char *name, double seconds,
                         size_t size, size_t iterations, uint64_t sum)
{
    const double bytes = (double)size * (double)iterations;
    const double gib = bytes / (1024.0 * 1024.0 * 1024.0);
    const double gib_s = gib / seconds;

    printf("%-12s time = %.6f s | throughput = %.3f GiB/s | checksum = 0x%016" PRIx64 "\n",
           name, seconds, gib_s, sum);
}

int main(int argc, char **argv)
{
    size_t size = 8u * 1024u * 1024u;
    size_t iterations = 256;

    if (argc > 1)
        size = (size_t)strtoull(argv[1], NULL, 0);
    if (argc > 2)
        iterations = (size_t)strtoull(argv[2], NULL, 0);

    uint8_t *buf = NULL;
    if (posix_memalign((void **)&buf, 64, size) != 0 || !buf) {
        perror("posix_memalign");
        return 1;
    }

    memset(buf, 0, size);

    uint64_t sum_scalar = 0;
    uint64_t sum_sse2 = 0;

    double t_scalar = bench(rle_fill_scalar, buf, size, iterations, 0x31, &sum_scalar);
    double t_sse2   = bench(rle_fill_sse2,   buf, size, iterations, 0x31, &sum_sse2);

    print_result("scalar", t_scalar, size, iterations, sum_scalar);
    print_result("sse2",   t_sse2,   size, iterations, sum_sse2);

    if (sum_scalar != sum_sse2) {
        fprintf(stderr, "ERRO: checksums diferentes.\n");
        free(buf);
        return 2;
    }

    printf("speedup      = %.3fx\n", t_scalar / t_sse2);

    free(buf);
    return 0;
}
