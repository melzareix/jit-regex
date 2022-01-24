#pragma once

#include <memory>
#include <vector>

#include "Automaton.h"
#include "Transition.h"

class AutomatonFactory {
private:
public:
  AutomatonFactory() = default;
  static Automaton* create_string_automaton(const std::string& str) {
    auto automaton = new Automaton();
    State s1;
    s1.is_accept() = true;
    automaton->initial() = s1.id();
    automaton->add_state(s1);
    automaton->set_accepted_str(str);
    return automaton;
  }

  static Automaton* create_kleene_star_automata(Automaton* exp) {
    auto exp_clone = new Automaton(*exp);
    exp_clone->expand();

    State s;
    s.is_accept() = true;
    s.add_eps_transitions(exp_clone->states[exp_clone->initial()]);

    for (auto p : exp_clone->accept_states()) {
      exp_clone->states[p].add_eps_transitions(s);
    }

    exp_clone->initial() = s.id();
    exp_clone->add_state(s);

    exp_clone->is_dfa() = false;

    return exp_clone;
  }

  static Automaton* create_union_automaton(Automaton* lhs, Automaton* rhs);
  static Automaton* create_union_automaton(std::vector<Automaton*> children);

  static Automaton* create_concat_automaton(Automaton* lhs, Automaton* rhs);
  static Automaton* create_concat_automaton(std::vector<Automaton*> children);

  static Automaton* create_char_range_automaton(char min, char max);
};
