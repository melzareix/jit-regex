#include "automaton/Automaton.h"

#include <list>
#include <vector>

void Automaton::add_state(const State& state) { states.insert({state.id(), state}); }

void Automaton::expand() {
  if (!is_singleton()) return;
  if (accepted_str->empty()) return;
  states.clear();

  std::vector<State> states_;
  size_t i;
  // n + 1 states
  for (i = 0; i <= accepted_str->size(); i++) {
    states_.emplace_back();
  }

  for (i = 0; i < states_.size() - 1; i++) {
    auto to = std::make_shared<State>(states_[i + 1]);
    states_[i].add_transition(Transition((*accepted_str)[i], to->id()));
    states.insert({states_[i].id(), states_[i]});
  }

  states_[i].is_accept() = true;
  states.insert({states_[i].id(), states_[i]});

  initial() = states_[0].id();
  is_dfa() = true;

  accepted_str.reset();  // reset the ptr no long singleton
}
std::vector<uint32_t> Automaton::accept_states() {
  std::vector<uint32_t> accepts;
  std::unordered_set<uint32_t> visited;
  std::list<uint32_t> work_list;

  work_list.push_back(initial());
  visited.insert(initial());
  while (!work_list.empty()) {
    State s = states[work_list.front()];
    work_list.pop_front();

    if (s.is_accept()) {
      accepts.push_back(s.id());
    }

    for (auto const& t : s.transitions()) {
      if (visited.count(t.dest()) == 0) {
        visited.insert(t.dest());
        work_list.push_back(t.dest());
      }
    }
  }

  return accepts;
}
std::unordered_set<uint32_t> Automaton::reachable_states() {
  std::unordered_set<uint32_t> visited;
  std::list<uint32_t> work_list;
  expand();

  work_list.push_back(initial());
  visited.insert(initial());

  while (!work_list.empty()) {
    State s = states[work_list.front()];
    work_list.pop_front();

    for (auto const& t : s.transitions()) {
      if (visited.count(t.dest()) == 0) {
        visited.insert(t.dest());
        work_list.push_back(t.dest());
      }
    }
  }
  return visited;
}
