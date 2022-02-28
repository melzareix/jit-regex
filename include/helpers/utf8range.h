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
#include <spdlog/spdlog.h>

#include <cstdint>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

#include "spdlog/fmt/fmt.h"
#include "utf8.h"

namespace ZRegex {
  struct UTF8SE {
    uint8_t start;
    uint8_t end;

    std::string Print() {
      if (start == end) {
        return fmt::format("[{:x}]", start);
      } else {
        return fmt::format("[{:x}-{:x}]", start, end);
      }
    }
  };
  struct ScalarRange {
    uint32_t start;
    uint32_t end;
    uint8_t encode_utf8(uint32_t code, std::vector<uint8_t>& dst) {
      auto const TAG_CONT = 0b10000000;
      auto const TAG_TWO_B = 0b11000000;
      auto const TAG_THREE_B = 0b11100000;
      auto const TAG_FOUR_B = 0b11110000;
      auto const MAX_ONE_B = 0x80;
      auto const MAX_TWO_B = 0x800;
      auto const MAX_THREE_B = 0x10000;
      auto res = 0;
      if (code < MAX_ONE_B && !dst.empty()) {
        dst[0] = code;
        res = 1;
      } else if (code < MAX_TWO_B && dst.size() >= 2) {
        dst[0] = (code >> 6 & 0x1F) | TAG_TWO_B;
        dst[1] = (code & 0x3F) | TAG_CONT;
        res = 2;
      } else if (code < MAX_THREE_B && dst.size() >= 3) {
        dst[0] = (code >> 12 & 0x0F) | TAG_THREE_B;
        dst[1] = (code >> 6 & 0x3F) | TAG_CONT;
        dst[2] = (code & 0x3F) | TAG_CONT;
        res = 3;
      } else if (dst.size() >= 4) {
        dst[0] = (code >> 18 & 0x07) | TAG_FOUR_B;
        dst[1] = (code >> 12 & 0x3F) | TAG_CONT;
        dst[2] = (code >> 6 & 0x3F) | TAG_CONT;
        dst[3] = (code & 0x3F) | TAG_CONT;
        res = 4;
      }
      return res;
    }
    std::optional<std::pair<ScalarRange, ScalarRange>> split() {
      if (start < 0xE000 && end > 0xD7FF) {
        return std::make_pair<ScalarRange, ScalarRange>({start, 0xD7FF}, {0xE000, end});
      }
      return std::nullopt;
    }

    bool is_valid() { return start <= end; }
    bool is_ascii() { return is_valid() && end <= 0x7f; }

    uint32_t encode(std::vector<uint8_t>& start_dst, std::vector<uint8_t>& end_dst) {
      auto ss = encode_utf8(start, start_dst);
      auto es = encode_utf8(end, end_dst);
      assert(ss == es);
      return ss;
    }
  };
  class UTF8Range {
  private:
    const uint8_t MAX_BYTES = 4;
    std::vector<ScalarRange> range_stack;

  public:
    UTF8Range(std::string& start, std::string& end) {
      auto s_u32
          = Utf8::ReadMultiByteCase(start, start[0], Utf8::multiByteSequenceLength(start[0]));
      auto e_u32 = Utf8::ReadMultiByteCase(end, end[0], Utf8::multiByteSequenceLength(end[0]));
      Push(s_u32, e_u32);
    }
    void Push(uint32_t start, uint32_t end) { range_stack.push_back({start, end}); }
    uint32_t MaxScalarValue(uint8_t n) {
      switch (n) {
        case 1:
          return 0x007F;
        case 2:
          return 0x07FF;
        case 3:
          return 0xFFFF;
        default:
          return 0x10FFFF;
      }
    }
    bool hasNext() { return !range_stack.empty(); }
    void PrintRange() {
    outer:
      while (!range_stack.empty()) {
        auto r = range_stack.back();
        range_stack.pop_back();
      inner:
        while (true) {
          auto res = r.split();
          if (res) {
            auto r1 = (*res).first, r2 = (*res).second;
            Push(r2.start, r2.end);
            r.start = r1.start;
            r.end = r1.end;
            goto inner;
          }

          if (!r.is_valid()) {
            goto outer;
          }

          for (size_t i = 1; i <= MAX_BYTES; i++) {
            auto max = MaxScalarValue(i);
            if (r.start <= max && max < r.end) {
              Push(max + 1, r.end);
              r.end = max;
              goto inner;
            }
          }

          if (r.is_ascii()) {
            auto rr = UTF8SE({r.start, r.end});
            spdlog::info("{}", rr.Print());
            return;
          }

          for (size_t i = 1; i <= MAX_BYTES; i++) {
            auto m = (1 << (6 * i)) - 1;
            if ((r.start & ~m) != (r.end & ~m)) {
              if ((r.start & m) != 0) {
                Push((r.start | m) + 1, r.end);
                r.end = r.start | m;
                goto inner;
              }
              if ((r.end & m) != m) {
                Push(r.end & ~m, r.end);
                r.end = (r.end & ~m) - 1;
                goto inner;
              }
            }
          }
          std::vector<uint8_t> start_dst(4);
          std::vector<uint8_t> end_dst(4);

          auto n = r.encode(start_dst, end_dst);
          std::vector<UTF8SE> rng;
          std::string out;
          for (size_t i = 0; i < n; i++) {
            rng.push_back({start_dst[i], end_dst[i]});
            out += rng.back().Print();
          }
          spdlog::info(out);
          return;
        }
      }
    }
  };

}  // namespace ZRegex
