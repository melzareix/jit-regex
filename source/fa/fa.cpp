//
// Created by Mohamed El Zarei on 12/02/2022.
//
#include <fa/fa.h>

#include <fstream>

#include "helpers/utf8.h"

namespace ZRegex {

  /**
   * Setters.
   */
  void FiniteAutomaton::SetInitial(std::shared_ptr<FiniteAutomatonState> initial) {
    initial_state = initial;
  }
  void FiniteAutomaton::SetDeterministic(bool det) { deterministic = det; }

  /**
   * Getters.
   */

  [[nodiscard]] fa_st FiniteAutomaton::GetStates() const {
    std::unordered_set<std::shared_ptr<FiniteAutomatonState>> visited;
    std::list<std::shared_ptr<FiniteAutomatonState>> worklist;

    worklist.push_back(initial_state);
    visited.insert(initial_state);

    while (!worklist.empty()) {
      auto s = worklist.front();
      worklist.pop_front();
      for (const auto& t : s->transitions) {
        if (visited.find(t.to) == visited.end()) {
          visited.insert(t.to);
          worklist.push_back(t.to);
        }
      }
    }

    return visited;
  }
  [[nodiscard]] fa_st FiniteAutomaton::GetAcceptStates() const {
    std::unordered_set<std::shared_ptr<FiniteAutomatonState>> accepts;

    std::unordered_set<std::shared_ptr<FiniteAutomatonState>> visited;
    std::list<std::shared_ptr<FiniteAutomatonState>> worklist;

    worklist.push_back(initial_state);
    visited.insert(initial_state);

    while (!worklist.empty()) {
      auto s = worklist.front();
      worklist.pop_front();
      if (s->accept) {
        accepts.insert(s);
      }

      for (const auto& t : s->transitions) {
        if (visited.find(t.to) == visited.end()) {
          visited.insert(t.to);
          worklist.push_back(t.to);
        }
      }
    }

    return accepts;
  }
  [[nodiscard]] std::vector<uint32_t> FiniteAutomaton::GetStartPoints() const {
    std::unordered_set<uint32_t> point_set;
    std::vector<uint32_t> points;
    point_set.insert(0);
    auto states = GetStates();
    for (const auto& s : states) {
      for (const auto& t : s->transitions) {
        point_set.insert(t.min);
        if (t.max < Utf8::MAX_TRANS) {
          point_set.insert(t.max + 1);
        }
      }
    }

    points.reserve(point_set.size());
    for (const auto& it : point_set) {
      points.push_back(it);
    }
    std::sort(points.begin(), points.end());
    return points;
  }

  /**
   * Operations.
   */
  void FiniteAutomaton::Complement(bool byte_dfa_utf8) {
    this->Determinize(byte_dfa_utf8);
    this->Totalize(byte_dfa_utf8);
    for (const auto& s : this->GetStates()) {
      s->SetAccept(!s->accept);
    }
    this->RemoveDeadStates();
  }

  void FiniteAutomaton::Visualize() const {
    std::ofstream fs("/tmp/regex.dot");
    fs << "digraph Automaton {" << std::endl;
    fs << "rankdir = LR;" << std::endl;
    auto states = GetStates();
    for (const auto& s : states) {
      fs << " " << s->id << " ";
      if (s->accept) {
        fs << "[shape=doublecircle,label=\"" << s->id << R"(",style=filled,fillcolor="green"];)"
           << std::endl;
      } else {
        fs << "[shape=circle,label=\"" << s->id << "\"];" << std::endl;
      }
      if (s->id == initial_state->id) {
        fs << "initial [shape=plaintext,label=\"\"];" << std::endl;
        fs << " initial -> " << s->id << std::endl;
      }

      for (const auto& t : s->transitions) {
        fs << " " << s->id;
        fs << " -> " << t.to->id << "[label=\"";
        if (t.min != t.max) {
          fs << fmt::format("0x{:x}", t.min) << "-" << fmt::format("0x{:x}", t.max);
        } else {
          fs << fmt::format("0x{:x}", t.min);
        }
        fs << "\"]" << std::endl;
      }
    }
    fs << "}" << std::endl;
    spdlog::debug("Dot Visualization of Regex");
    // system("cat /tmp/regex.dot");
  }
  fa_st FiniteAutomaton::GetLiveStates() const {
    std::unordered_map<FiniteAutomatonState, fa_st, FiniteAutomaton::StateHasher> map;
    for (auto s : GetStates()) {
      map[*s] = fa_st();
    }
    for (auto s : GetStates()) {
      for (auto t : s->transitions) {
        map[*t.to].insert(s);
      }
    }

    fa_st live_states = GetAcceptStates();
    std::list<std::shared_ptr<FiniteAutomatonState>> worklist;
    for (auto s : live_states) {
      worklist.push_back(s);
    }
    while (!worklist.empty()) {
      auto s = worklist.front();
      worklist.pop_front();
      for (auto p : map[*s]) {
        if (live_states.find(p) == live_states.end()) {
          live_states.insert(p);
          worklist.push_back(p);
        }
      }
    }

    return live_states;
  }
  void FiniteAutomaton::Reduce() {
    auto states = GetStates();
    for (auto s : states) {
      auto st = s->GetSortedTransitions();
      s->ResetTransitions();
      uint32_t min = -1, max = -1;
      std::shared_ptr<FiniteAutomatonState> p;
      for (auto t : st) {
        if (p == t.to) {
          if (t.min <= max + 1) {
            if (t.max > max) {
              max = t.max;
            }
          } else {
            if (p != nullptr) {
              s->AddTransition(min, max, p);
            }
            min = t.min;
            max = t.max;
          }
        } else {
          if (p != nullptr) {
            s->AddTransition(min, max, p);
          }
          p = t.to;
          min = t.min;
          max = t.max;
        }
      }
      if (p != nullptr) {
        s->AddTransition(min, max, p);
      }
    }
  }

  void FiniteAutomaton::RemoveDeadStates() {
    auto states = GetStates();
    auto live = GetLiveStates();
    for (auto s : states) {
      auto ts = s->transitions;
      s->ResetTransitions();
      for (auto t : ts) {
        if (live.find(t.to) != live.end()) {
          s->AddTransition(t.min, t.max, t.to);
        }
      }
    }
    Reduce();
  }
  std::map<uint32_t, std::vector<FiniteAutomatonTransition>>
  FiniteAutomaton::GetSortedTransitions() {
    std::map<uint32_t, std::vector<FiniteAutomatonTransition>> transitions;
    for (auto& s : GetStates()) {
      transitions[s->id] = s->GetSortedTransitions();
    }
    return transitions;
  }

  std::vector<std::vector<FiniteAutomatonTransition>>
  FiniteAutomaton::GetSortedTransitionsVector() {
    SetStateNumbers();
    auto states = GetStates();
    std::vector<std::vector<FiniteAutomatonTransition>> transitions(states.size());
    for (auto& s : GetStates()) {
      transitions[s->id] = s->GetSortedTransitions();
    }
    return transitions;
  }

  void FiniteAutomaton::Totalize(bool byte_dfa_utf8) {
    auto s = std::make_shared<FiniteAutomatonState>();
    uint32_t max;
    if (byte_dfa_utf8) {
      max = Utf8::MAX_TRANS_BYTE;
    } else {
      max = Utf8::MAX_TRANS;
    }

    s->AddTransition(0, max, s);
    auto states = this->GetStates();
    for (const auto& p : states) {
      uint32_t maxi = 0;
      for (auto t : p->GetSortedTransitions()) {
        if (t.min > maxi) {
          p->AddTransition(maxi, t.min - 1, s);
        }
        if (t.max + 1 > maxi) {
          maxi = t.max + 1;
        }
      }
      if (maxi <= max) {
        p->AddTransition(maxi, max, s);
      }
    }
  }
  void FiniteAutomaton::Determinize(bool byte_dfa_utf8) {
    if (this->deterministic) return;
    auto points = GetStartPoints();
    std::list<fa_st> worklist;
    std::unordered_map<fa_st, std::shared_ptr<FiniteAutomatonState>, FiniteAutomaton::Hasher>
        new_state;
    fa_st initial_set = {initial_state};
    worklist.push_back(initial_set);

    initial_state = std::make_shared<FiniteAutomatonState>();
    initial_state->chr = initial_state->chr;
    new_state[initial_set] = initial_state;

    auto iter = 0;
    while (!worklist.empty()) {
      // probably will fail fallback
      if (iter > 3000) {
        throw std::runtime_error("Pattern too large! Potential Exponential State Explosion!");
      }
      auto s = worklist.front();
      worklist.pop_front();
      auto r = new_state[s];
      //        auto r = std::make_shared<FiniteAutomatonState>();

      // new superset is accepting if one of the substates is accepting
      for (const auto& q : s) {
        if (q->accept) {
          r->SetAccept(true);
          break;
        }
      }

      for (size_t i = 0; i < points.size(); ++i) {
        fa_st p;
        for (const auto& q : s) {
          for (const auto& t : q->transitions) {
            if (t.min <= points[i] && points[i] <= t.max) {
              p.insert(t.to);
            }
          }
        }

        std::shared_ptr<FiniteAutomatonState> q;
        if (!p.empty()) {
          if (new_state.count(p) == 0) {
            worklist.push_back(p);
            q = new_state[p] = std::make_shared<FiniteAutomatonState>();
            if (p.size() == 1) q->chr = (p.begin())->get()->chr;
          } else {
            q = new_state[p];
          }

          uint32_t min = points[i];
          uint32_t max;

          if (i + 1 < points.size()) {
            max = points[i + 1] - 1;
          } else {
            if (byte_dfa_utf8) {
              max = Utf8::MAX_TRANS_BYTE;
            } else {
              max = Utf8::MAX_TRANS;
            }
          }

          r->AddTransition(min, max, q);
        }
      }
      iter++;
    }
    // spdlog::error("iters: {}", iter);
    this->SetDeterministic(true);
    this->RemoveDeadStates();
  }

  void FiniteAutomaton::SetStateNumbers() {
    auto states = GetStates();
    auto number = 0;
    for (auto& s : states) {
      s->id = number++;
    }
  }
  // Copy Constructor

  FiniteAutomaton::FiniteAutomaton(const FiniteAutomaton& a) {
    // spdlog::debug("FA Copy Constructor");
    deterministic = a.deterministic;
    std::unordered_map<uint32_t, std::shared_ptr<FiniteAutomatonState>> map;
    auto states = a.GetStates();
    for (const auto& s : states) {
      map[s->id] = std::make_shared<FiniteAutomatonState>();
    }

    for (const auto& s : states) {
      auto p = map[s->id];
      p->SetAccept(s->accept);
      if (s->id == a.initial_state->id) {
        SetInitial(p);
      }
      for (const auto& t : s->transitions) {
        p->AddTransition(t.min, t.max, map[t.to->id]);
      }
    }
  }

}  // namespace ZRegex
