//
// Created by Mohamed El Zarei on 02/01/2022.
//

#ifndef ZREGEXSTANDALONE_TRANSITION_H
#define ZREGEXSTANDALONE_TRANSITION_H

#include <cstdint>
#include <memory>

namespace ZRegex {
  class FiniteAutomatonState;

  class FiniteAutomatonTransition {
  private:
  public:
    uint16_t min, max;
    std::shared_ptr<FiniteAutomatonState> to;

    FiniteAutomatonTransition(uint16_t min, uint16_t max, std::shared_ptr<FiniteAutomatonState> to)
        : min(min), max(max), to(to){};
    FiniteAutomatonTransition(uint16_t c, std::shared_ptr<FiniteAutomatonState> to)
        : FiniteAutomatonTransition(c, c, std::move(to)){};

    friend inline bool operator==(const FiniteAutomatonTransition& t1, const FiniteAutomatonTransition& t2);

    struct HashFunction {
      size_t operator()(const FiniteAutomatonTransition& t) const {
        return t.min * 2 + t.max * 3;
      }
    };
  };

  bool operator==(const FiniteAutomatonTransition& t1, const FiniteAutomatonTransition& t2) {
    return t1.min == t2.min && t1.max == t2.max && t1.to == t2.to;
  }
}  // namespace ZRegex
#endif  // ZREGEXSTANDALONE_TRANSITION_H
