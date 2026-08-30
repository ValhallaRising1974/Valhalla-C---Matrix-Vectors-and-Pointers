#include "rle_fill.h"

__attribute__((noinline))
void rle_fill_sse2(uint8_t *dst, uint8_t value, size_t count)
{
    if (count == 0)
        return;

    const uint32_t pattern = (uint32_t)value * 0x01010101u;
    const size_t simd_bytes = count & ~(size_t)15;
    uint8_t *p = dst;
    uint8_t *end = dst + simd_bytes;

    if (simd_bytes != 0) {
        __asm__ volatile (
            "movd   %[pat], %%xmm0\n\t"
            "pshufd $0, %%xmm0, %%xmm0\n\t"
            "1:\n\t"
            "cmp    %[end], %[ptr]\n\t"
            "jae    2f\n\t"
            "movdqu %%xmm0, (%[ptr])\n\t"
            "add    $16, %[ptr]\n\t"
            "jmp    1b\n\t"
            "2:\n\t"
            : [ptr] "+r" (p)
            : [end] "r" (end),
              [pat] "r" (pattern)
            : "xmm0", "cc", "memory"
        );
    }

    for (size_t i = simd_bytes; i < count; ++i) {
        dst[i] = value;
    }
}
