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
#include <string>
#include <vector>

namespace ZRegex {
  class KMPAlgorithm {
  private:
    std::vector<uint32_t> table;
    const std::string pattern_;
    void BuildTable() {
      for (int i = 1, k = 0; i < pattern_.size(); ++i) {
        while (k && pattern_[k] != pattern_[i]) k = table[k - 1];
        if (pattern_[k] == pattern_[i]) ++k;
        table[i] = k;
      }
    }

  public:
    KMPAlgorithm(const std::string& pattern) : pattern_(pattern) {
      table.resize(pattern.size(), 0);
      BuildTable();
    }
    uint8_t Search(const std::string& haystack) {
      for (int i = 0, k = 0; i < haystack.size(); ++i) {
        while (k && pattern_[k] != haystack[i]) k = table[k - 1];
        if (pattern_[k] == haystack[i]) ++k;
        if (k == pattern_.size()) return 1;
      }
      return 0;
    }
  };
}  // namespace ZRegex
