//
// Created by Mohamed El Zarei on 13/12/2021.
//

#include "automaton/AutomatonFactory.h"

Automaton* AutomatonFactory::create_concat_automaton(Automaton* lhs, Automaton* rhs) {
  if (lhs->is_singleton() && rhs->is_singleton()) {
    return AutomatonFactory::create_string_automaton(lhs->get_accepted_str()
                                                     + rhs->get_accepted_str());
  }

  bool dfa = (lhs->is_singleton() || lhs->is_dfa()) && rhs->is_dfa();

  Automaton* lhs_cln;
  Automaton* rhs_cln;

  // clone and expand
  lhs_cln = new Automaton(*lhs);
  lhs_cln->expand();

  rhs_cln = new Automaton(*rhs);
  rhs_cln->expand();

  for (auto s : lhs_cln->accept_states()) {
    lhs_cln->states[s].is_accept() = false;
    auto to_state = rhs_cln->states[rhs_cln->initial()];
    lhs_cln->states[s].add_eps_transitions(to_state);
    // need to add the rhs states to the lhs states different from dk
    // where the transitions include the full states not just ids
  }

  lhs_cln->states.insert(rhs_cln->states.begin(), rhs_cln->states.end());

  lhs_cln->is_dfa() = dfa;
  return lhs_cln;
}
Automaton* AutomatonFactory::create_concat_automaton(std::vector<Automaton*> children) {
  return nullptr;
}

Automaton* AutomatonFactory::create_union_automaton(Automaton* lhs, Automaton* rhs) {
  return nullptr;
}
Automaton* AutomatonFactory::create_union_automaton(std::vector<Automaton*> children) {
  return nullptr;
}

Automaton* AutomatonFactory::create_char_range_automaton(char min, char max) {
  if (min == max) {
    auto s = std::string(1, min);
    return AutomatonFactory::create_string_automaton(s);
  }
  auto automaton = new Automaton();
  State s1;
  State s2;
  s2.is_accept() = true;

  if (max > min) {
    s1.add_transition(Transition(min, max, s2.id()));
  }
  automaton->initial() = s1.id();
  automaton->add_state(s1);
  automaton->add_state(s2);
  return automaton;
}
