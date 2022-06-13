/*
 Copyright (c) 2022 Mohamed Elzarei

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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

namespace ZRegex {
  class SIMDSubstringMatch {
  private:
    char* pattern_;

  public:
    static void p512_hex_u8(__m512i in) {
      alignas(64) uint8_t v[64];
      _mm512_store_si512((__m512i*)v, in);
      printf(
          "v64_u8: %x %x %x %x | %x %x %x %x | %x %x %x %x | %x %x %x %x | %x %x %x %x | %x %x %x "
          "%x | %x %x %x %x | %x %x %x %x | %x %x %x %x | %x %x %x %x | %x %x %x %x | %x %x %x %x "
          "| %x %x %x %x | %x %x %x "
          "%x | %x %x %x %x | %x %x %x %x\n",
          v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13],
          v[14], v[15], v[16], v[17], v[18], v[19], v[20], v[21], v[22], v[23], v[24], v[25], v[26],
          v[27], v[28], v[29], v[30], v[31], v[32], v[33], v[34], v[35], v[36], v[37], v[38], v[39],
          v[40], v[41], v[42], v[43], v[44], v[45], v[46], v[47], v[48], v[49], v[50], v[51], v[52],
          v[53], v[54], v[55], v[56], v[57], v[58], v[59], v[60], v[61], v[62], v[63]);
    }

    typedef union {
      __m128i v;
      unsigned int ui[4];
      unsigned short int us[8];
      unsigned char uc[16];
    } VectorUnion;

    static __m512i* preprocess(const char* p, uint32_t m) {
      auto alpha = sizeof(__m512i);
      auto m_prime = m;

      char** b = new char*[m_prime];
      auto b512 = new __m512i[m_prime];

      for (size_t i = 0; i < m_prime; i++) {
        b[i] = new char[alpha];
      }

      // preprocessing
      for (size_t i = 0; i < m_prime; i++) {
        for (size_t j = 0; j < alpha; j++) {
          b[i][j] = p[i];
        }
        b512[i] = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(b[i]));
      }
      clean_b(b, m);
      return b512;
    }

    static inline uint16_t wsmatch_128(__m128i a, __m128i b) {
      VectorUnion Z;
      Z.ui[0] = Z.ui[1] = Z.ui[2] = Z.ui[3] = 0;

      auto h = _mm_mpsadbw_epu8(a, b, 0x00);
      auto d = _mm_cmpeq_epi8(h, Z.v);
      // auto d = _mm_cmpeq_epi16(h, z);
      return _mm_movemask_epi8(d);
    }

    static inline uint32_t wsmatch_256(__m256i a, __m256i b) {
      auto h = _mm256_mpsadbw_epu8(a, b, 0);
      auto z = _mm256_setzero_si256();
      return _mm256_cmpeq_epi8_mask(h, z);
    }

    static uint32_t epsm_a(const char* p, uint32_t m, const char* ss, uint32_t n, __m512i* b) {
      auto alpha = sizeof(__m512i);
      auto m_prime = m;

      for (size_t i = 0; i < n; i += alpha) {
        uint64_t r = -1;  // same effect as 0xFFFFFFFFFFFFFFFF;
        const __m512i t = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(ss + i));
        for (size_t j = 0; j < m_prime; j++) {
          auto s = _mm512_cmpeq_epi8_mask(b[j], t);
          auto sh = s >> j;
          r = r & sh;
          if (r == 0) break;  // no point in continuing if r is zero
        }
        if (r > 0 && i + __builtin_ctzl(r) <= n) {
          return 1;
        } else {
          if (i < 1) continue;

          for (size_t j = 0; i > 0 && j <= m && i >= j; j++) {
            if (memcmp(ss + i - j, p, m) == 0) return 1;
          }
        }
      }

      return 0;
    }

    static inline void clean_b(char** b, uint32_t m) {
      // Delete the array created
      for (int i = 0; i < m; i++)  // To delete the inner arrays
        delete[] b[i];
      delete[] b;  // To delete the outer array
    }

    //   SIMDSubstringMatch(char* pattern) : pattern_(pattern) {}
    template <typename T>
    static unsigned get_first_bit_set(const T value) {
      assert(value != 0);

      return __builtin_ctz(value);
    }

    template <typename T>
    static T clear_leftmost_set(const T value) {
      assert(value != 0);

      return value & (value - 1);
    }

    static size_t sse4_strstr_anysize(const char* s, size_t n, const char* needle, size_t k) {
      assert(n > 0);
      assert(k > 0);

      const __m512i first = _mm512_set1_epi8(needle[0]);
      const __m512i last = _mm512_set1_epi8(needle[k - 1]);

      char* haystack = const_cast<char*>(s);
      char* end = haystack + n;

      for (/**/; haystack < end; haystack += 64) {
        const __m512i block_first = _mm512_loadu_si512(haystack + 0);
        const __m512i block_last = _mm512_loadu_si512(haystack + k - 1);

        uint64_t mask
            = _mm512_cmpeq_epi8_mask(block_first, first) & _mm512_cmpeq_epi8_mask(block_last, last);

        while (mask != 0) {
          const uint64_t bitpos = get_first_bit_set(mask);
          const char* s = reinterpret_cast<const char*>(haystack);

          // spdlog::warn("{} {}", s + bitpos + 1, needle + 1);
          if (memcmp(s + bitpos + 1, needle + 1, k - 2) == 0) {
            return 1;
          }

          mask = clear_leftmost_set(mask);
        }
      }

      return 0;
    }
  };
};  // namespace ZRegex