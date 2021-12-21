#include "automaton/Automaton.h"

#include <fstream>
#include <list>
#include <nlohmann/json.hpp>
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

std::vector<uint16_t> Automaton::get_start_points() {
  std::unordered_set<uint16_t> point_set;
  expand();
  for (auto s : states) {
    for (auto t : s.second.transitions()) {
      point_set.insert(t.min());
      point_set.insert(t.max() + 1);
    }
  }

  std::vector<uint16_t> points;
  points.insert(points.end(), point_set.begin(), point_set.end());
  std::sort(points.begin(), points.end());

  return points;
}
void Automaton::to_dfa() {
  //  if (this->is_dfa() || this->is_singleton()) return;
  //  std::unordered_set<uint32_t> initial_set;
  //  initial_set.insert(initial());
  //
  //  std::vector<uint16_t> points = this->get_start_points();
  //  std::list<std::unordered_set<uint32_t>> work_list;
  //
  //  std::unordered_map<std::unordered_set<uint32_t>, uint32_t> new_state;
  //  work_list.push_back(initial_set);
  //  auto new_initial =  State();
  //  states[new_initial.id()] = new_initial;
  //
  //  this->initial() = new_initial.id();
  //  new_state.insert({initial_set, new_initial.id()});
  //
  //  while (!work_list.empty()) {
  ////    State s = states[work_list.front()];
  //  }
}
Automaton* Automaton::from_json(std::ifstream& ifs) {
  nlohmann::json j = nlohmann::json::parse(ifs);
  auto states = j["states"];
  auto accept = j["acceptStates"];
  auto is = j["initialState"];

  auto* automaton = new Automaton();

  // is
  automaton->initial() = is["id"];

  // accept states
  std::unordered_set<nlohmann::json> acceptset;
  for (auto as : accept) {
    auto i = as["id"];
    acceptset.insert(i);
  }


  // states
  for (auto as : states) {
    State s(as["id"]);
    if (acceptset.count((as["id"])) > 0) {
      s.is_accept() = true;
    }

    for (auto t : as["transitions"]) {
      auto mn = t["min"];
      auto max = t["max"];
      auto to = t["to"];
      s.add_transition(Transition(mn, max, to));
    }
    automaton->add_state(s);
  }


  return automaton;
}
