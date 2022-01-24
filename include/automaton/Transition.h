#pragma once
#include <cstdint>
#include <memory>
#include <utility>

class State;

class Transition {
private:
  // Range for the transition (min = max in case transition only on a single char).
  uint16_t min_;
  uint16_t max_;
  uint32_t to;  // dest state

public:
  Transition(uint16_t min, uint16_t max, uint32_t to) : min_(min), max_(max), to(to){};
  Transition(uint16_t c, uint32_t to) : Transition(c, c, to){};

  [[nodiscard]] const uint16_t& min() const { return min_; }
  [[nodiscard]] const uint16_t& max() const { return max_; }

  [[nodiscard]] uint32_t dest() const { return to; }

  friend inline bool operator==(const Transition& t1, const Transition& t2);

  struct HashFunction {
    size_t operator()(const Transition& t) const {
      return t.min() * 2 + t.max() * 3 + t.dest() * 4;
    }
  };
};

bool operator==(const Transition& t1, const Transition& t2) {
  return t1.min() == t2.min() && t1.max() == t2.max() && t1.dest() == t2.dest();
}