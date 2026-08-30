#include "rle_fill.h"

__attribute__((noinline))
void rle_fill_scalar(uint8_t *dst, uint8_t value, size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        dst[i] = value;
    }
}
