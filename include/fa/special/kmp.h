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
#include <memory>
#include <string>
#include <vector>

#include "fa/fa.h"

namespace ZRegex {
  class KMPAlgorithm {
  private:
    std::vector<uint32_t> table;
    const std::string pattern_;

  public:
    KMPAlgorithm(const std::string& pattern) : pattern_(pattern) {}
    void BuildTable() {
      table.resize(pattern_.size(), 0);
      for (int i = 1, k = 0; i < pattern_.size(); ++i) {
        while (k && pattern_[k] != pattern_[i]) k = table[k - 1];
        if (pattern_[k] == pattern_[i]) ++k;
        table[i] = k;
      }
    }
    std::unique_ptr<FiniteAutomaton> asDfa() {
      // dfa as array
      auto m = pattern_.size();
      std::vector<std::vector<uint32_t>> dfa(m, std::vector<uint32_t>(256));
      dfa[0][(unsigned char)pattern_[0]] = 1;

      for (uint32_t x = 0, j = 1; j < m; j++) {
        for (uint32_t c = 0; c < 256; c++) {
          dfa[j][c] = dfa[x][c];
        }
        dfa[j][(unsigned char)pattern_[j]] = j + 1;
        x = dfa[x][(unsigned char)pattern_[j]];
      }

      // convert to dfa obj
      std::vector<std::shared_ptr<FiniteAutomatonState>> states(pattern_.size() + 1);
      for (size_t i = 0; i < states.size(); i++) {
        states[i] = std::make_shared<FiniteAutomatonState>();
      }
      auto fa = std::make_unique<FiniteAutomaton>(states[0]);
      for (uint32_t j = 0; j < m; j++) {
        for (uint32_t c = 0; c < 256; c++) {
          // if (dfa[j][c] == 0) continue;
          // if (pattern_.find((char)c) == std::string::npos) continue;
          states[j]->AddTransition(c, c, states[dfa[j][c]]);
        }
      }
      fa->SetDeterministic(true);
      states[m]->SetAccept(true);
      auto as = fa->GetAcceptStates();
      fa->RemoveDeadStates();
      // fa->Visualize();
      return std::move(fa);
    }
    uint8_t Search(const std::string& haystack) {
      uint8_t num_matches = 0;
      for (int i = 0, k = 0; i < haystack.size(); ++i) {
        while (k && pattern_[k] != haystack[i]) k = table[k - 1];
        if (pattern_[k] == haystack[i]) ++k;
        if (k == pattern_.size()) {
          return 1;  // single match only
          num_matches++;
          k = table[k - 1];
        }
      }
      return num_matches;
    }
  };
}  // namespace ZRegex
