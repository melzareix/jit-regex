#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#if defined(__has_include) && (__has_include(<x86intrin.h>))
#  include <x86intrin.h>
#else
#  error "upgrade your compiler. it's free..."
#endif
#include "spdlog/sinks/rotating_file_sink.h"

typedef union {
  __m128i* symbol16;
  char* symbol;
} TEXT;

typedef union {
  __m128i v;
  unsigned int ui[4];
  unsigned short int us[8];
  char uc[16];
} VectorUnion;

typedef struct list {
  struct list* next;
  int pos;
} LIST;

int epsm2_search(const char* pattern, uint32_t patlen, const char* x, uint32_t textlen) {
  //   if (patlen < 2) return search1(pattern, patlen, x, textlen);
  //   if (patlen == 2) return search2(pattern, patlen, x, textlen);
  //   if (patlen == 3) return search3(pattern, patlen, x, textlen);
  //   if (patlen == 4) return search4(pattern, patlen, x, textlen);
  //   if (patlen >= 16) return search16(pattern, patlen, x, textlen);
  assert(patlen < 16);
  char* y0;
  int i, j, k, count = 0;
  VectorUnion P, zero;
  __m128i res, a, b, z, p;

  __m128i* text = (__m128i*)x;
  __m128i* tend = (__m128i*)(x + 16 * (textlen / 16));
  tend--;

  //   BEGIN_PREPROCESSING
  zero.ui[0] = zero.ui[1] = zero.ui[2] = zero.ui[3] = 0;
  z = zero.v;
  P.uc[0] = pattern[patlen - 5];
  P.uc[1] = pattern[patlen - 4];
  P.uc[2] = pattern[patlen - 3];
  P.uc[3] = pattern[patlen - 2];
  p = P.v;

  i = (patlen - 1) / 16;  // i points the first 16-byte block that P may end in
  i++;
  text += i;
  for (k = 0; k < (i * 16 + 8) - patlen + 1; k++)
    if (0 == memcmp(pattern, x + k, patlen)) return 1;
  //   END_PREPROCESSING

  //   BEGIN_SEARCHING
  // the loop checks if pattern ends at the second half of text[i] or at the first half of text[i+1]
  while (text < tend) {
    // check if P[(m-5) ... (m-2)] matches with T[i*16+4 ... i*16+7], T[i*16+5 ... i*16+8], .... ,
    // T[i*16+11 ... i*16+14] note thet this corresponds P ends at
    // T[i*16+8],T[i*16+9],...,T[i*16+15]
    res = _mm_mpsadbw_epu8(*text, p, 0x04);
    b = _mm_cmpeq_epi16(res, z);
    j = _mm_movemask_epi8(b);
    if (j) {
      y0 = (char*)(text) + 9 - patlen;
      if ((j & 3) == 3 && !memcmp(pattern, y0, patlen)) return 1;
      if ((j & 12) == 12 && !memcmp(pattern, y0 + 1, patlen)) return 1;
      if ((j & 48) == 48 && !memcmp(pattern, y0 + 2, patlen)) return 1;
      if ((j & 192) == 192 && !memcmp(pattern, y0 + 3, patlen)) return 1;
      if ((j & 768) == 768 && !memcmp(pattern, y0 + 4, patlen)) return 1;
      if ((j & 3072) == 3072 && !memcmp(pattern, y0 + 5, patlen)) return 1;
      if ((j & 12288) == 12288 && !memcmp(pattern, y0 + 6, patlen)) return 1;
      if ((j & 49152) == 49152 && !memcmp(pattern, y0 + 7, patlen)) return 1;
    }

    a = _mm_blend_epi16(*text, *(text + 1), 0x0f);
    b = _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2));

    // check if P ends at T[(i+1)*16+8],T[(i+1)*16+9],...,T[(i+1)*16+15]
    res = _mm_mpsadbw_epu8(b, p, 0x04);
    b = _mm_cmpeq_epi16(res, z);
    j = _mm_movemask_epi8(b);

    if (j) {
      y0 = (char*)(text) + 9 + 8 - patlen;
      if ((j & 3) == 3 && !memcmp(pattern, y0, patlen)) return 1;
      if ((j & 12) == 12 && !memcmp(pattern, y0 + 1, patlen)) return 1;
      if ((j & 48) == 48 && !memcmp(pattern, y0 + 2, patlen)) return 1;
      if ((j & 192) == 192 && !memcmp(pattern, y0 + 3, patlen)) return 1;
      if ((j & 768) == 768 && !memcmp(pattern, y0 + 4, patlen)) return 1;
      if ((j & 3072) == 3072 && !memcmp(pattern, y0 + 5, patlen)) return 1;
      if ((j & 12288) == 12288 && !memcmp(pattern, y0 + 6, patlen)) return 1;
      if ((j & 49152) == 49152 && !memcmp(pattern, y0 + 7, patlen)) return 1;
    }
    text++;
  }

  for (k = ((char*)text) + 8 - x; k < textlen; k++) {
    if (0 == memcmp(pattern, &x[k - patlen + 1], patlen)) return 1;
  }
  //   END_SEARCHING
  return count;
}