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
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#if defined(__has_include) && (__has_include(<x86intrin.h>))
#  include <x86intrin.h>
#else
#  error "upgrade your compiler. it's free..."
#endif

namespace ZRegex {
  class SIMDSubstringMatch {
  private:
    char* pattern_;

  public:
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

    static size_t sse4_strstr_anysize(const char* s, size_t n, const char* needle,
                                      size_t needle_size) {
      assert(needle_size > 4);
      assert(n > 0);

      const __m128i prefix = _mm_loadu_si128(reinterpret_cast<const __m128i*>(needle));
      const __m128i zeros = _mm_setzero_si128();

      for (size_t i = 0; i < n; i += 8) {
        const __m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(s + i));
        const __m128i result = _mm_mpsadbw_epu8(data, prefix, 0);

        const __m128i cmp = _mm_cmpeq_epi16(result, zeros);

        unsigned mask = _mm_movemask_epi8(cmp) & 0x5555;

        while (mask != 0) {
          const auto bitpos = get_first_bit_set(mask) / 2;

          if (std::memcmp(s + i + bitpos + 4, needle + 4, needle_size - 4) == 0) {
            return 1;
          }

          mask = clear_leftmost_set(mask);
        }
      }

      return 0;
    }
  };
};  // namespace ZRegex