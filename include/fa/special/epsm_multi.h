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

namespace ZRegex {
  class EPSMMatcher {
  public:
    const char* pattern;
    uint32_t patlen;
    VectorUnion P, zero;
    __m128i z, p;
    __m512i* b512;
    bool pdone = false;

    EPSMMatcher(const char* pattern, uint32_t patlen) : pattern(pattern), patlen(patlen) {
      zero.ui[0] = zero.ui[1] = zero.ui[2] = zero.ui[3] = 0;
      z = zero.v;
      P.uc[0] = pattern[patlen - 5];
      P.uc[1] = pattern[patlen - 4];
      P.uc[2] = pattern[patlen - 3];
      P.uc[3] = pattern[patlen - 2];
      p = P.v;
    }
    static inline void clean_b(char** b, uint32_t m) {
      // Delete the array created
      for (int i = 0; i < m; i++)  // To delete the inner arrays
        delete[] b[i];
      delete[] b;  // To delete the outer array
    }
    void preprocess() {
      auto alpha = sizeof(__m512i);
      auto m_prime = patlen;

      char** b = new char*[m_prime];
      b512 = new __m512i[m_prime];

      for (size_t i = 0; i < m_prime; i++) {
        b[i] = new char[alpha];
      }

      // preprocessing
      for (size_t i = 0; i < m_prime; i++) {
        for (size_t j = 0; j < alpha; j++) {
          b[i][j] = pattern[i];
        }
        b512[i] = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(b[i]));
      }
      clean_b(b, patlen);
      pdone = true;
    }

    int epsma_elz_early(const char* ss, uint32_t n) {
      if (!pdone) preprocess();
      auto alpha = sizeof(__m512i);
      auto m_prime = patlen;
      int ret = -1;

      for (size_t i = 0; i < n; i += alpha) {
        uint64_t r = -1;  // same effect as 0xFFFFFFFFFFFFFFFF;
        const __m512i t = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(ss + i));
        for (size_t j = 0; j < m_prime; j++) {
          auto s = _mm512_cmpeq_epi8_mask(b512[j], t);
          auto sh = s >> j;
          r = r & sh;
          if (r == 0) break;  // no point in continuing if r is zero
        }
        if (r > 0 && i + __builtin_ctzl(r) <= n) {
          return alpha * i + __builtin_ctzl(r);
        } else {
          if (i < 1) continue;

          for (size_t j = 0; i > 0 && j <= patlen && i >= j; j++) {
            if (memcmp(ss + i - j, pattern, patlen) == 0) return i - j;
          }
        }
      }

      return ret;
    }

    int epsma_elz(const char* ss, uint32_t n) {
      if (!pdone) preprocess();
      auto alpha = sizeof(__m512i);
      auto m_prime = patlen;
      int ret = -1;

      for (size_t i = 0; i < n; i += alpha) {
        uint64_t r = -1;  // same effect as 0xFFFFFFFFFFFFFFFF;
        const __m512i t = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(ss + i));
        for (size_t j = 0; j < m_prime; j++) {
          auto s = _mm512_cmpeq_epi8_mask(b512[j], t);
          auto sh = s >> j;
          r = r & sh;
          if (r == 0) break;  // no point in continuing if r is zero
        }
        if (r > 0 && i + __builtin_ctzl(r) <= n) {
          auto rr = r;
          uint64_t p = 0;
          while (rr != 0) {
            auto q = __builtin_ffsl(rr);
            if (p + q - 1 >= n) break;
            p += (q - 1);
            rr = rr >> q;
          }

          ret = alpha * i + p;
        } else {
          if (i < 1) continue;

          for (size_t j = 0; i > 0 && j <= patlen && i >= j; j++) {
            if (memcmp(ss + i - j, pattern, patlen) == 0) ret = i - j;
          }
        }
      }

      return ret;
    }
    int epsm2_search_find_last(const char* x, uint32_t textlen) {
      char* y0;
      uint32_t ret = -1;
      int i, j, k = 0;
      __m128i res, a, b, z;

      __m128i* text = (__m128i*)x;
      __m128i* tend = (__m128i*)(x + 16 * (textlen / 16));
      tend--;

      //   BEGIN_PREPROCESSING
      int tidx = 0;
      i = (patlen - 1) / 16;  // i points the first 16-byte block that P may end in
      i++;
      text += i;
      tidx += i;
      for (k = 0; k < (i * 16 + 8) - patlen + 1; k++)
        if (0 == memcmp(pattern, x + k, patlen)) ret = k;
      //   END_PREPROCESSING

      //   BEGIN_SEARCHING
      // the loop checks if pattern ends at the second half of text[i] or at the first half of
      // text[i+1]
      while (text < tend) {
        // check if P[(m-5) ... (m-2)] matches with T[i*16+4 ... i*16+7], T[i*16+5 ... i*16+8], ....
        // , T[i*16+11 ... i*16+14] note thet this corresponds P ends at
        // T[i*16+8],T[i*16+9],...,T[i*16+15]
        res = _mm_mpsadbw_epu8(*text, p, 0x04);
        b = _mm_cmpeq_epi16(res, z);
        j = _mm_movemask_epi8(b);
        if (j) {
          y0 = (char*)(text) + 9 - patlen;
          if ((j & 3) == 3 && !memcmp(pattern, y0, patlen)) ret = tidx + 9 - patlen;
          if ((j & 12) == 12 && !memcmp(pattern, y0 + 1, patlen)) ret = tidx + 9 - patlen + 1;
          if ((j & 48) == 48 && !memcmp(pattern, y0 + 2, patlen)) ret = tidx + 9 - patlen + 2;
          if ((j & 192) == 192 && !memcmp(pattern, y0 + 3, patlen)) ret = tidx + 9 - patlen + 3;
          if ((j & 768) == 768 && !memcmp(pattern, y0 + 4, patlen)) ret = tidx + 9 - patlen + 4;
          if ((j & 3072) == 3072 && !memcmp(pattern, y0 + 5, patlen)) ret = tidx + 9 - patlen + 5;
          if ((j & 12288) == 12288 && !memcmp(pattern, y0 + 6, patlen)) ret = tidx + 9 - patlen + 6;
          if ((j & 49152) == 49152 && !memcmp(pattern, y0 + 7, patlen)) ret = tidx + 9 - patlen + 7;
        }

        a = _mm_blend_epi16(*text, *(text + 1), 0x0f);
        b = _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2));

        // check if P ends at T[(i+1)*16+8],T[(i+1)*16+9],...,T[(i+1)*16+15]
        res = _mm_mpsadbw_epu8(b, p, 0x04);
        b = _mm_cmpeq_epi16(res, z);
        j = _mm_movemask_epi8(b);

        if (j) {
          y0 = (char*)(text) + 9 + 8 - patlen;
          if ((j & 3) == 3 && !memcmp(pattern, y0, patlen)) ret = tidx + 9 + 8 - patlen;
          if ((j & 12) == 12 && !memcmp(pattern, y0 + 1, patlen)) ret = tidx + 9 + 8 - patlen + 1;
          if ((j & 48) == 48 && !memcmp(pattern, y0 + 2, patlen)) ret = tidx + 9 + 8 - patlen + 2;
          if ((j & 192) == 192 && !memcmp(pattern, y0 + 3, patlen)) ret = tidx + 9 + 8 - patlen + 3;
          if ((j & 768) == 768 && !memcmp(pattern, y0 + 4, patlen)) ret = tidx + 9 + 8 - patlen + 4;
          if ((j & 3072) == 3072 && !memcmp(pattern, y0 + 5, patlen))
            ret = tidx + 9 + 8 - patlen + 5;
          if ((j & 12288) == 12288 && !memcmp(pattern, y0 + 6, patlen))
            ret = tidx + 9 + 8 - patlen + 6;
          if ((j & 49152) == 49152 && !memcmp(pattern, y0 + 7, patlen))
            ret = tidx + 9 + 8 - patlen + 7;
        }
        text++;
        tidx++;
      }

      for (k = ((char*)text) + 8 - x; k < textlen; k++) {
        if (0 == memcmp(pattern, &x[k - patlen + 1], patlen)) ret = tidx + k - patlen + 1;
      }
      //   END_SEARCHING
      return ret;
    }

    int epsm2_search_find_first(const char* x, uint32_t textlen) {
      char* y0;
      int i, j, k = 0;
      VectorUnion P, zero;
      __m128i res, a, b, z, p;

      __m128i* text = (__m128i*)x;
      __m128i* tend = (__m128i*)(x + 16 * (textlen / 16));
      tend--;

      //   BEGIN_PREPROCESSING
      int tidx = 0;
      i = (patlen - 1) / 16;  // i points the first 16-byte block that P may end in
      i++;
      text += i;
      tidx += i;
      for (k = 0; k < (i * 16 + 8) - patlen + 1; k++)
        if (0 == memcmp(pattern, x + k, patlen)) return k;
      //   END_PREPROCESSING

      //   BEGIN_SEARCHING
      // the loop checks if pattern ends at the second half of text[i] or at the first half of
      // text[i+1]
      while (text < tend) {
        // check if P[(m-5) ... (m-2)] matches with T[i*16+4 ... i*16+7], T[i*16+5 ... i*16+8], ....
        // , T[i*16+11 ... i*16+14] note thet this corresponds P ends at
        // T[i*16+8],T[i*16+9],...,T[i*16+15]
        res = _mm_mpsadbw_epu8(*text, p, 0x04);
        b = _mm_cmpeq_epi16(res, z);
        j = _mm_movemask_epi8(b);
        if (j) {
          y0 = (char*)(text) + 9 - patlen;
          if ((j & 3) == 3 && !memcmp(pattern, y0, patlen)) return tidx + 9 - patlen;
          if ((j & 12) == 12 && !memcmp(pattern, y0 + 1, patlen)) return tidx + 9 - patlen + 1;
          if ((j & 48) == 48 && !memcmp(pattern, y0 + 2, patlen)) return tidx + 9 - patlen + 2;
          if ((j & 192) == 192 && !memcmp(pattern, y0 + 3, patlen)) return tidx + 9 - patlen + 3;
          if ((j & 768) == 768 && !memcmp(pattern, y0 + 4, patlen)) return tidx + 9 - patlen + 4;
          if ((j & 3072) == 3072 && !memcmp(pattern, y0 + 5, patlen)) return tidx + 9 - patlen + 5;
          if ((j & 12288) == 12288 && !memcmp(pattern, y0 + 6, patlen))
            return tidx + 9 - patlen + 6;
          if ((j & 49152) == 49152 && !memcmp(pattern, y0 + 7, patlen))
            return tidx + 9 - patlen + 7;
        }

        a = _mm_blend_epi16(*text, *(text + 1), 0x0f);
        b = _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2));

        // check if P ends at T[(i+1)*16+8],T[(i+1)*16+9],...,T[(i+1)*16+15]
        res = _mm_mpsadbw_epu8(b, p, 0x04);
        b = _mm_cmpeq_epi16(res, z);
        j = _mm_movemask_epi8(b);

        if (j) {
          y0 = (char*)(text) + 9 + 8 - patlen;
          if ((j & 3) == 3 && !memcmp(pattern, y0, patlen)) return tidx + 9 + 8 - patlen;
          if ((j & 12) == 12 && !memcmp(pattern, y0 + 1, patlen)) return tidx + 9 + 8 - patlen + 1;
          if ((j & 48) == 48 && !memcmp(pattern, y0 + 2, patlen)) return tidx + 9 + 8 - patlen + 2;
          if ((j & 192) == 192 && !memcmp(pattern, y0 + 3, patlen))
            return tidx + 9 + 8 - patlen + 3;
          if ((j & 768) == 768 && !memcmp(pattern, y0 + 4, patlen))
            return tidx + 9 + 8 - patlen + 4;
          if ((j & 3072) == 3072 && !memcmp(pattern, y0 + 5, patlen))
            return tidx + 9 + 8 - patlen + 5;
          if ((j & 12288) == 12288 && !memcmp(pattern, y0 + 6, patlen))
            return tidx + 9 + 8 - patlen + 6;
          if ((j & 49152) == 49152 && !memcmp(pattern, y0 + 7, patlen))
            return tidx + 9 + 8 - patlen + 7;
        }
        text++;
        tidx++;
      }

      for (k = ((char*)text) + 8 - x; k < textlen; k++) {
        if (0 == memcmp(pattern, &x[k - patlen + 1], patlen)) return tidx + k - patlen + 1;
      }
      //   END_SEARCHING
      return -1;
    }
  };

}  // namespace ZRegex
