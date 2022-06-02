#pragma once
#include <x86intrin.h>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

template <typename T>
static inline T clear_leftmost_set_512(const T value) {
  assert(value != 0);

  return value & (value - 1);
}

template <typename T>
static inline unsigned get_first_bit_set(const T value) {
  assert(value != 0);

  return __builtin_ctz(value);
}

__mmask16 inline zero_byte_mask(const __m512i v) {
  const __m512i v01 = _mm512_set1_epi8(0x01);
  const __m512i v80 = _mm512_set1_epi8(int8_t(0x80));

  const __m512i v1 = _mm512_sub_epi32(v, v01);
  // tmp1 = (v - 0x01010101) & ~v & 0x80808080
  const __m512i tmp1 = _mm512_ternarylogic_epi32(v1, v, v80, 0x20);

  return _mm512_test_epi32_mask(tmp1, tmp1);
}

size_t avx2_strstr_anysize(const char* s, size_t n, const char* needle, size_t k) {
  const __m256i first = _mm256_set1_epi8(needle[0]);
  const __m256i last = _mm256_set1_epi8(needle[k - 1]);

  for (size_t i = 0; i < n; i += 32) {
    const __m256i block_first = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(s + i));
    const __m256i block_last = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(s + i + k - 1));

    const __m256i eq_first = _mm256_cmpeq_epi8(first, block_first);
    const __m256i eq_last = _mm256_cmpeq_epi8(last, block_last);

    uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(eq_first, eq_last));

    while (mask != 0) {
      const auto bitpos = get_first_bit_set(mask);

      if ((i + bitpos + k - 1) < n && std::memcmp(s + i + bitpos + 1, needle + 1, k - 2) == 0) {
        return i + bitpos;
      }

      mask = clear_leftmost_set_512(mask);
    }
  }

  return std::string::npos;
}

size_t avx512f_strstr_long(const char* string, size_t n, const char* needle, size_t k) {
  __m512i curr;
  __m512i next;
  __m512i v0, v1, v2, v3;

  char* haystack = const_cast<char*>(string);
  char* last = haystack + n;

  const uint32_t prf = *(uint32_t*)needle;  // the first 4 bytes of needle
  const __m512i prefix = _mm512_set1_epi32(prf);

  next = _mm512_loadu_si512(haystack);

  for (/**/; haystack < last; haystack += 64) {
    curr = next;
    next = _mm512_loadu_si512(haystack + 64);
    const __m512i shft = _mm512_alignr_epi32(next, curr, 1);

    v0 = curr;

    {
      const __m512i t1 = _mm512_srli_epi32(curr, 8);
      const __m512i t2 = _mm512_slli_epi32(shft, 24);
      v1 = _mm512_or_si512(t1, t2);
    }
    {
      const __m512i t1 = _mm512_srli_epi32(curr, 16);
      const __m512i t2 = _mm512_slli_epi32(shft, 16);
      v2 = _mm512_or_si512(t1, t2);
    }
    {
      const __m512i t1 = _mm512_srli_epi32(curr, 24);
      const __m512i t2 = _mm512_slli_epi32(shft, 8);
      v3 = _mm512_or_si512(t1, t2);
    }

    uint16_t m0 = _mm512_cmpeq_epi32_mask(v0, prefix);
    uint16_t m1 = _mm512_cmpeq_epi32_mask(v1, prefix);
    uint16_t m2 = _mm512_cmpeq_epi32_mask(v2, prefix);
    uint16_t m3 = _mm512_cmpeq_epi32_mask(v3, prefix);

    int index = 64;
    while (m0 | m1 | m2 | m3) {
      if (m0) {
        int pos = __builtin_ctz(m0) * 4 + 0;
        m0 = m0 & (m0 - 1);

        if (pos < index && memcmp(haystack + pos + 4, needle + 4, k - 4) == 0) {
          index = pos;
        }
      }

      if (m1) {
        int pos = __builtin_ctz(m1) * 4 + 1;
        m1 = m1 & (m1 - 1);

        if (pos < index && memcmp(haystack + pos + 4, needle + 4, k - 4) == 0) {
          index = pos;
        }
      }

      if (m2) {
        int pos = __builtin_ctz(m2) * 4 + 2;
        m2 = m2 & (m2 - 1);

        if (pos < index && memcmp(haystack + pos + 4, needle + 4, k - 4) == 0) {
          index = pos;
        }
      }

      if (m3) {
        int pos = __builtin_ctz(m3) * 4 + 3;
        m3 = m3 & (m3 - 1);

        if (pos < index && memcmp(haystack + pos + 4, needle + 4, k - 4) == 0) {
          index = pos;
        }
      }
    }

    if (index < 64) {
      return 1;
    }
  }

  return 0;
}

uint32_t avx2_strstr_anysize_single(const char* s, size_t n, const char* needle, size_t k) {
  auto r = avx2_strstr_anysize(s, n, needle, k);
  return r == std::string::npos ? 0 : 1;
}

size_t avx512f_strstr_v2_anysize(const char* string, size_t n, const char* needle, size_t k) {
  //   assert(n > 0);
  //   assert(k > 0);

  const __m512i first = _mm512_set1_epi8(needle[0]);
  const __m512i last = _mm512_set1_epi8(needle[k - 1]);

  char* haystack = const_cast<char*>(string);
  char* end = haystack + n;

  for (/**/; haystack < end; haystack += 64) {
    const __m512i block_first = _mm512_loadu_si512(haystack + 0);
    const __m512i block_last = _mm512_loadu_si512(haystack + k - 1);

#if 0
        const __m512i first_zeros = _mm512_xor_si512(block_first, first);
        const __m512i last_zeros  = _mm512_xor_si512(block_last, last);
        const __m512i zeros       = _mm512_or_si512(first_zeros, last_zeros);
#else
    const __m512i first_zeros = _mm512_xor_si512(block_first, first);
    /*
        first_zeros | block_last | last |  first_zeros | (block_last ^ last)
        ------------+------------+------+------------------------------------
             0      |      0     |   0  |      0
             0      |      0     |   1  |      1
             0      |      1     |   0  |      1
             0      |      1     |   1  |      0
             1      |      0     |   0  |      1
             1      |      0     |   1  |      1
             1      |      1     |   0  |      1
             1      |      1     |   1  |      1
    */
    const __m512i zeros = _mm512_ternarylogic_epi32(first_zeros, block_last, last, 0xf6);
#endif

    uint32_t mask = zero_byte_mask(zeros);
    while (mask) {
      const uint64_t p = __builtin_ctz(mask);

      if ((4 * p + k + 0) < n && std::memcmp(haystack + 4 * p + 0, needle, k) == 0) {
        return 1;
      }

      if ((8 * k + 1) < n && std::memcmp(haystack + 4 * p + 1, needle, k) == 0) {
        return 1;
      }

      if ((8 * k + 2) < n && std::memcmp(haystack + 4 * p + 2, needle, k) == 0) {
        return 1;
      }

      if ((8 * k + 3) < n && std::memcmp(haystack + 4 * p + 3, needle, k) == 0) {
        return 1;
      }

      mask = clear_leftmost_set_512(mask);
    }
  }

  return 0;
}
