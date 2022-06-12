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
  class EPSMMatcher {
  public:
    std::string pattern;
    __m512i* b512;
    bool pdone = false;

    EPSMMatcher(std::string& pat) : pattern(pat) {}

    static inline void clean_b(char** b, uint32_t m) {
      // Delete the array created
      for (int i = 0; i < m; i++)  // To delete the inner arrays
        delete[] b[i];
      delete[] b;  // To delete the outer array
    }
    void preprocess() {
      auto alpha = sizeof(__m512i);
      auto m_prime = pattern.size();

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
      clean_b(b, pattern.size());
      pdone = true;
    }

    int epsma_elz_early(const char* ss, uint32_t n) {
      if (!pdone) preprocess();
      auto alpha = sizeof(__m512i);
      auto m_prime = pattern.size();
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

          for (size_t j = 0; i > 0 && j <= pattern.size() && i >= j; j++) {
            if (memcmp(ss + i - j, pattern.c_str(), pattern.size()) == 0) return i - j;
          }
        }
      }

      return ret;
    }

    int epsma_elz(const char* ss, uint32_t n) {
      if (!pdone) preprocess();
      auto alpha = sizeof(__m512i);
      auto m_prime = pattern.size();
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

          for (size_t j = 0; i > 0 && j <= pattern.size() && i >= j; j++) {
            if (memcmp(ss + i - j, pattern.c_str(), pattern.size()) == 0) ret = i - j;
          }
        }
      }

      return ret;
    }
  };
}  // namespace ZRegex
