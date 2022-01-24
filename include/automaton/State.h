#pragma once
#include <cstdint>
#include <unordered_set>

#include "../automaton/Transition.h"

class State {
private:
  inline static uint32_t ts_counter = 0;  // global counter for created states ids

  bool is_accept_;
  uint32_t ts_;
  uint32_t id_;
  std::unordered_set<Transition, Transition::HashFunction> transitions_;

public:
  explicit State(uint32_t id) : id_(id) {
    ts_ = State::ts_counter++;
    is_accept_ = false;
  };
  State() {
    ts_ = State::ts_counter++;
    is_accept_ = false;
    id_ = ts_;
  };
  void clear() { transitions_.clear(); }

  bool& is_accept() { return is_accept_; }
  [[nodiscard]] const bool& is_accept() const { return is_accept_; }

  [[nodiscard]] auto& transitions() const { return transitions_; }
  void add_transition(const Transition& t) { transitions_.insert(t); }

  void add_eps_transitions(const State& to) {
    if (to.is_accept()) {
      is_accept() = true;
    }
    transitions_.insert(to.transitions_.begin(), to.transitions_.end());
  }

  [[nodiscard]] uint32_t& ts() { return ts_; }
  [[nodiscard]] const uint32_t& ts() const { return ts_; }
  [[nodiscard]] const uint32_t& id() const { return id_; }

  friend inline bool operator==(const State& s1, const State& s2);

  struct HashFunction {
    size_t operator()(const State& s) const { return s.ts(); }
  };
};

bool operator==(const State& s1, const State& s2) { return s1.ts() == s2.ts(); }
