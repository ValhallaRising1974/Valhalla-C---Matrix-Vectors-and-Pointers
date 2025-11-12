#ifndef LYRA_ASM_H
#define LYRA_ASM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Retorna o tamanho de uma string C (exclui o '\0')
size_t fast_strlen(const char *s);

// memxor(dst, src, n): dst[i] ^= src[i] para i em [0, n)
void memxor(uint8_t *dst, const uint8_t *src, size_t n);

#ifdef __cplusplus
}
#endif
#endif
