#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "State.h"

class Automaton {
private:
  std::shared_ptr<std::string> accepted_str;
  bool is_dfa_;

  uint32_t initial_;

public:
  std::unordered_map<uint32_t, State> states;

  Automaton() {
    is_dfa_ = true;
    initial_ = -1;
    states.clear();
  }

  bool is_singleton() { return accepted_str != nullptr; }

  [[nodiscard]] const bool& is_dfa() const { return is_dfa_; }
  bool& is_dfa() { return is_dfa_; }

  uint32_t& initial() { return initial_; }
  [[nodiscard]] const uint32_t& initial() const { return initial_; }

  void add_state(const State& state);
  std::vector<uint32_t> accept_states();

  std::unordered_set<uint32_t> reachable_states();

  std::string get_accepted_str() { return *accepted_str; }
  void set_accepted_str(const std::string& str) {
    if (accepted_str == nullptr) {
      accepted_str = std::make_shared<std::string>(str);
    } else {
      throw std::runtime_error("Trying to mutate the automaton.");
    }
  }

  /**
   * Expand the Automaton if it's a singleton.
   * E.g. a (q1)-[hello]-(q2) becomes (q1)->h->(q2)-->(e)...
   */
  void expand();

  void to_dfa();
  std::vector<uint16_t> get_start_points();

  static std::unique_ptr<Automaton> from_json(std::ifstream& ifs);
};
