//
// Created by Mohamed El Zarei on 02/01/2022.
//
#include <fa/state.h>

namespace ZRegex {
  bool operator==(const FiniteAutomatonState& s1, const FiniteAutomatonState& s2) {
    return s1.id == s2.id;
  }
  bool operator<(const FiniteAutomatonState& s1, const FiniteAutomatonState& s2) {
    return s1.id < s2.id;
  }
}  // namespace ZRegex